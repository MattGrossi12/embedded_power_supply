# Site do projeto

Portfólio técnico estático da fonte de bancada. Segue a metodologia de apresentação utilizada no projeto [RSA_APB](https://mattgrossi12.github.io/RSA_APB_32-bit/): abertura com imagens reais, navegação por âncoras, arquitetura, construção, evidências e documentação.

## Gerar e visualizar

Requer Python 3.10 ou mais recente. Não utiliza pacotes externos, framework, Node.js ou CDN.

Na raiz do repositório:

```powershell
python scripts/pages/build.py --output _site
python -m http.server 8000 --bind 127.0.0.1 --directory _site
```

Acesse <http://localhost:8000/>. Use `Ctrl+C` para encerrar o servidor. Abra a saída gerada, pois `site/index.html` contém marcadores preenchidos pelo gerador.

## Fontes e saída

| Caminho | Responsabilidade |
| --- | --- |
| `site/index.html` | Estrutura e textos da apresentação |
| `site/assets/style.css` | Tema, layout e comportamento responsivo |
| `site/assets/app.js` | Ampliação de imagens, navegação ativa e reprodução de vídeos |
| `scripts/pages/build.py` | Catálogo, tabelas, cópia dos artefatos e validação |
| `00_Imagens` | Imagens reais do projeto e figuras da monografia |
| `05_Ensaios_e_resultados/Videos` | Registros em vídeo incluídos automaticamente |
| `_site` | Saída gerada, ignorada pelo Git |
| `.github/workflows/pages.yml` | Geração e publicação no GitHub Pages |

O gerador verifica a existência dos artefatos, assinaturas PNG/JPEG, links locais, âncoras e IDs únicos. O arquivo gerado `assets/inventario.json` associa cada artefato à origem, descrição, etapa, ferramenta ou documento e hash SHA-256. As cópias com sufixo `_copia` não são duplicadas no site.

As tabelas de resultados reproduzem a monografia, seções 4.1 a 4.3; a pinagem reproduz as Figuras 43 a 45, p. 64. Esses dados não representam novos ensaios. O gerador não compila o firmware nem verifica o funcionamento do circuito. Os vídeos são identificados pelos arquivos, sem atribuir condições de ensaio que seus nomes não documentam.

Os links de navegação, imagens originais e vídeos funcionam sem JavaScript. A ampliação utiliza diálogo nativo com fechamento por Escape e restauração de foco. A interface respeita a preferência por movimento reduzido.

## Publicação

O workflow publica a saída gerada quando as alterações chegam à branch `main`, ou por execução manual. No repositório do GitHub, configure **Settings → Pages → Source → GitHub Actions**. O endereço esperado é `https://mattgrossi12.github.io/embedded_power_supply/`, quando o workflow concluir a publicação.

Todos os recursos internos usam caminhos relativos para funcionar nesse subdiretório. Os documentos e vídeos são incluídos no artefato; links para os fontes completos abrem o repositório.

## Revisão

Após gerar, confira desktop e celular, links, imagens, diálogo, rolagem local das tabelas e reprodução dos vídeos. A validação automática de caminhos não substitui a revisão visual. A data de geração ou de modificação de um arquivo não determina a data do ensaio original.
