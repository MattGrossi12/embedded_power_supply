"""Generate the static portfolio from repository artifacts, using Python's stdlib."""
from __future__ import annotations

import argparse
import hashlib
import html
import json
from pathlib import Path
import re
import shutil
from urllib.parse import quote, unquote, urlsplit
from html.parser import HTMLParser

ROOT = Path(__file__).resolve().parents[2]
REPO = "https://github.com/MattGrossi12/embedded_power_supply"
SECTIONS = [("visao-geral", "Visão geral"), ("arquitetura", "Arquitetura"),
            ("construcao", "Construção"), ("interface", "Interface e firmware"),
            ("pinagem", "Pinagem"), ("ensaios", "Ensaios"), ("documentos", "Documentação")]


def esc(value):
    return html.escape(str(value), quote=True)


class SiteBuilder:
    def __init__(self, output):
        self.output = output
        self.inventory = {}

    def copy(self, relative, destination, description, stage, tool):
        source = ROOT / relative
        if not source.is_file():
            raise FileNotFoundError(f"Missing project artifact: {relative}")
        signature = source.read_bytes()[:16]
        suffix = source.suffix.lower()
        if suffix == ".png" and not signature.startswith(b"\x89PNG\r\n\x1a\n"):
            raise ValueError(f"File extension does not match PNG content: {relative}")
        if suffix in (".jpg", ".jpeg") and not signature.startswith(b"\xff\xd8\xff"):
            raise ValueError(f"File extension does not match JPEG content: {relative}")
        target = self.output / destination
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        self.inventory[destination] = {
            "arquivo": destination, "origem": relative, "conteudo": description,
            "etapa": stage, "ferramenta_ou_documento": tool,
            "sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
        }
        return quote(destination, safe="/")

    def figure(self, name, title, caption, stage="Desenvolvimento", tool="Registro fotográfico do projeto"):
        url = self.copy(f"00_Imagens/{name}", f"media/{name}", f"{title}. {caption}", stage, tool)
        return (f'<figure class="technical-figure"><a href="{url}" data-image '
                f'aria-label="Ampliar: {esc(title)}"><img src="{url}" alt="{esc(title)}" '
                f'loading="lazy" decoding="async"></a><figcaption><strong>{esc(title)}</strong>'
                f'{esc(caption)}</figcaption></figure>')

    def table(self, title, headers, rows, raw=False):
        head = "".join(f'<th scope="col">{esc(x)}</th>' for x in headers)
        body = "".join('<tr>' + ''.join(f'<td>{x if raw else esc(x)}</td>' for x in row) + '</tr>' for row in rows)
        return f'<div class="table-scroll" role="region" aria-label="{esc(title)}" tabindex="0"><table><caption>{esc(title)}</caption><thead><tr>{head}</tr></thead><tbody>{body}</tbody></table></div>'

    def run(self):
        self.output.mkdir(parents=True, exist_ok=True)
        for source in (ROOT / "site/assets").iterdir():
            if source.is_file():
                target = self.output / "assets" / source.name
                target.parent.mkdir(exist_ok=True)
                shutil.copy2(source, target)
        f = self.figure
        values = {}
        for key, css in [("HERO_NAV", "hero"), ("SIDE_NAV", "side")]:
            values[key] = ''.join(f'<a href="#{ident}"><span>{i:02}</span>{esc(label)}</a>' for i, (ident, label) in enumerate(SECTIONS, 1))
        values["PROTOTYPE"] = f("prototipo_fonte_gabinete.jpeg", "Montagem do protótipo no gabinete", "Vista do conjunto com display TFT, botões de comando e bornes de saída instalados no painel frontal.", "Integração")
        values["CIRCUIT"] = f("figura_39_circuito_principal.png", "Circuito da fonte principal", "Figura 39 da monografia, p. 60. Fonte: elaborado pelo autor (2024).", "Projeto do circuito", "Monografia / Proteus")
        build = [
            ("layout_placa_principal.jpeg", "Leiaute da placa principal", "Distribuição das trilhas, dos terminais de conexão e dos componentes no projeto da placa de circuito impresso.", "Projeto da PCB", "Proteus"),
            ("placa_principal_trilhas.jpeg", "Placa principal após a formação das trilhas", "Face de cobre da placa antes da instalação dos componentes, com identificação do projeto e da instituição.", "Fabricação", "Registro fotográfico"),
            ("placa_principal_montagem.jpeg", "Montagem dos componentes da fonte principal", "Instalação do banco de cinco capacitores eletrolíticos, regulador LM723, resistores, trimpot e bornes de conexão.", "Montagem", "Registro fotográfico"),
            ("placa_principal_componentes.jpeg", "Disposição dos componentes e terminais", "Vista da placa principal durante a montagem, com os bornes destinados à interligação com os demais circuitos da fonte.", "Montagem", "Registro fotográfico"),
            ("placas_fontes_auxiliares.jpeg", "Montagem das fontes auxiliares", "Conjunto de quatro placas com capacitores, resistores, diodos, trimpots e bornes de conexão instalados.", "Montagem", "Registro fotográfico"),
            ("pcb_fonte_5v_render.png", "Representação 3D da fonte de 5 V", "Renderização da PCB disponível nos arquivos do projeto.", "Projeto da PCB", "Proteus"),
        ]
        values["BUILD_GALLERY"] = ''.join(f(*row) for row in build)
        values["AUX_GALLERY"] = ''.join(f(f"apendice_{letter}_fonte_{suffix}.png", f"Fonte auxiliar de {volts} V", f"Apêndice {letter}, p. {page} da monografia. Fonte: Matheus Grossi (2024).", "Projeto do circuito", "Monografia") for letter, suffix, volts, page in [("B", "3v3", "3,3", 82), ("C", "5v", "5", 83), ("D", "12v", "12", 84), ("E", "24v", "24", 85)])
        values["INTERFACE_GALLERY"] = f("figura_48_menus_da_fonte.png", "Estrutura dos menus da interface", "Figura 48, p. 67 (página 68 do PDF). Tela de abertura, menu principal e opções de comando da fonte regulável.", "Interface", "Monografia") + f("2024-10-20_215023_display_TFT_duty_cycle_50_porcento.jpeg", "Indicação do ciclo de trabalho do PWM", "Registro experimental de outubro de 2024, com indicação de ciclo de trabalho de 50% no display.", "Ensaio da interface")
        values["FIRMWARE_TABLE"] = self.table("Configuração do firmware selecionado", ["Função", "Configuração"], [("Subir / aumentar", "GPIO 22"), ("Descer / diminuir", "GPIO 18"), ("Confirmar", "GPIO 19"), ("Saída PWM", "GPIO 5"), ("Frequência PWM", "5 kHz"), ("Resolução PWM", "12 bits · 0 a 4095")])
        values["PINOUT_TABLES"] = self.table("Dados · Figura 43", ["SSD1963", "ESP32"], [(f"D{i}", f"GPIO {pin}") for i, pin in enumerate([12, 13, 26, 25, 17, 16, 27, 14])]) + self.table("Alimentação · Figura 44", ["SSD1963", "Alimentação"], [("5V", "5V"), ("3.3V", "3.3V"), ("LED_A", "5V"), ("GND", "GND")]) + self.table("Controle · Figura 45", ["SSD1963", "ESP32"], [("CS", "GPIO 33"), ("RS", "GPIO 15"), ("RESET", "GPIO 32"), ("WR", "GPIO 04"), ("RD", "GPIO 02")])
        values["CONNECTOR"] = f("pinagem_conector_display_tft.jpg", "Identificação do conector do TFT", "Serigrafia dos sinais e da alimentação no verso do módulo.", "Interface")
        results = [("10%", "3,2 V", "2,9 V", "−9,4%"), ("50%", "16,0 V", "17,0 V", "+6,25%"), ("100%", "32,0 V", "31,9 V", "−0,31%")]
        values["RESULTS_TABLE"] = self.table("Resultados da monografia · seções 4.1 a 4.3, pp. 70–72", ["Ajuste", "Esperada", "Medida", "Desvio relativo"], results)
        values["TEST_GALLERY"] = ''.join(f(f"figura_{number}_teste_{percent}_porcento.png", f"Ensaio com ajuste de {percent}%", f"Figura {number}, p. {page}. Tensão medida: {voltage}. Fonte: elaborado pelo autor (2024).", "Ensaios", "Monografia") for number, percent, page, voltage in [(51, 10, 70, "2,9 V"), (52, 50, 71, "17,0 V"), (53, 100, 72, "31,9 V")])
        videos = sorted((ROOT / "05_Ensaios_e_resultados/Videos").rglob("*.mp4"))
        if not videos:
            raise ValueError("No test videos found")
        rows = []
        for i, video in enumerate(videos, 1):
            label = f"Registro de bancada {i:02}"
            url = self.copy(video.relative_to(ROOT).as_posix(), f"videos/{video.name}", label + "; condição de ajuste não identificada pelo nome do arquivo", "Ensaios", "Vídeo do projeto")
            rows.append([esc(label), f'<a href="{url}" data-video="{esc(label)}">Reproduzir</a>', f'<a href="{url}">{esc(video.name)}</a>'])
        values["VIDEO_TABLE"] = self.table("Vídeos dos testes", ["Registro", "Reprodução", "Arquivo original"], rows, raw=True)
        for relative, destination, label in [
            ("01_Texto_academico/Monografia/TCC Matheus Grossi-Rev.03-11-2024.pdf", "documents/monografia.pdf", "Monografia de Matheus Grossi, 2024"),
            ("05_Ensaios_e_resultados/Planilhas/Comparacao_Vout_calculada_testada_R1_R2.xlsx", "documents/comparacao_tensoes.xlsx", "Planilha de comparação de tensão"),
            ("README.md", "documents/README.md", "Snapshot do README da geração"),
        ]:
            self.copy(relative, destination, label, "Documentação", "Arquivo original do repositório")
        # Hero reuses real project artifacts, including the prototype already copied.
        self.copy("00_Imagens/2024-10-20_215023_display_TFT_duty_cycle_50_porcento.jpeg", "media/2024-10-20_215023_display_TFT_duty_cycle_50_porcento.jpeg", "Indicador de duty cycle em 50%", "Ensaio da interface", "Registro fotográfico")
        template = (ROOT / "site/index.html").read_text(encoding="utf-8")
        for key, value in values.items():
            marker = "{{" + key + "}}"
            if marker not in template:
                raise ValueError(f"Unused content: {key}")
            template = template.replace(marker, value)
        if re.search(r"\{\{\w+\}\}", template):
            raise ValueError("Unresolved template markers")
        (self.output / "index.html").write_text(template, encoding="utf-8")
        (self.output / ".nojekyll").touch()
        inventory = {"repositorio": REPO, "contexto": "Artefatos documentais do projeto de 2024. A geração do site não revalida os circuitos ou o firmware.", "arquivos": list(self.inventory.values())}
        (self.output / "assets/inventario.json").write_text(json.dumps(inventory, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        validate(self.output)
        print(f"Site generated: {self.output} ({len(self.inventory)} artifacts; {len(videos)} videos)")


class LinkParser(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.links, self.ids, self.images = [], [], []

    def handle_starttag(self, tag, attrs):
        attrs = dict(attrs)
        if "id" in attrs:
            self.ids.append(attrs["id"])
        for key in ("href", "src"):
            if attrs.get(key):
                self.links.append(attrs[key])
        if tag == "img":
            if "alt" not in attrs:
                raise ValueError("Image without alt text")
            self.images.append(attrs.get("src"))


def validate(output):
    parser = LinkParser()
    parser.feed((output / "index.html").read_text(encoding="utf-8"))
    if len(parser.ids) != len(set(parser.ids)):
        raise ValueError("Duplicate section IDs")
    for link in parser.links:
        url = urlsplit(link)
        if url.scheme or url.netloc:
            continue
        if url.path and not (output / unquote(url.path)).is_file():
            raise FileNotFoundError(f"Broken local link: {link}")
        if not url.path and url.fragment and url.fragment not in parser.ids:
            raise ValueError(f"Broken anchor: {link}")
        if url.path.startswith("/") or ":\\" in url.path:
            raise ValueError(f"Nonportable path: {link}")
    for section, _ in SECTIONS:
        if section not in parser.ids:
            raise ValueError(f"Missing section: {section}")
    print(f"Validated {len(parser.links)} links and {len(parser.images)} image elements")


if __name__ == "__main__":
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--output", default="_site", help="Generated output directory; separate from source files")
    args = ap.parse_args()
    output = Path(args.output).resolve()
    # Never overwrite source folders with generated content.
    if output == ROOT or output in [ROOT / name for name in ("site", "scripts", "00_Imagens", ".git")]:
        ap.error("Choose a separate output directory")
    SiteBuilder(output).run()
