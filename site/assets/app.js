/* Links, anchors and original images remain available without JavaScript. */
const viewer = document.querySelector('#image-viewer');
let origin;
if (viewer && typeof viewer.showModal === 'function') {
  const close = () => viewer.close();
  document.querySelectorAll('[data-image]').forEach(link => {
    link.addEventListener('click', event => {
      if (event.ctrlKey || event.metaKey || event.shiftKey || event.altKey) return;
      event.preventDefault();
      origin = link;
      const image = document.querySelector('#viewer-image');
      image.src = link.href;
      image.alt = link.querySelector('img').alt;
      document.querySelector('#viewer-caption').textContent = link.closest('figure').querySelector('figcaption').textContent;
      document.querySelector('#viewer-original').href = link.href;
      viewer.showModal();
    });
  });
  document.querySelector('#viewer-close').addEventListener('click', close);
  viewer.addEventListener('click', event => { if (event.target === viewer) { const r = viewer.getBoundingClientRect(); if (event.clientX < r.left || event.clientX > r.right || event.clientY < r.top || event.clientY > r.bottom) close(); } });
  viewer.addEventListener('close', () => { document.querySelector('#viewer-image').removeAttribute('src'); origin?.focus(); });
}
const player = document.querySelector('#test-video');
document.querySelectorAll('[data-video]').forEach(link => {
  link.addEventListener('click', event => {
    if (event.ctrlKey || event.metaKey || event.shiftKey || event.altKey) return;
    event.preventDefault();
    player.src = link.href;
    player.load();
    document.querySelector('#video-status').textContent = link.dataset.video;
    player.scrollIntoView({ block: 'center', behavior: matchMedia('(prefers-reduced-motion: reduce)').matches ? 'instant' : 'smooth' });
    player.play().catch(() => { document.querySelector('#video-status').textContent += ' — utilize os controles para iniciar a reprodução.'; });
  });
});
if ('IntersectionObserver' in window) {
  const links = [...document.querySelectorAll('.sidebar nav a')];
  const observer = new IntersectionObserver(entries => entries.forEach(entry => {
    if (entry.isIntersecting) links.forEach(link => {
      const active = link.hash === '#' + entry.target.id;
      link.classList.toggle('active', active);
      if (active) link.setAttribute('aria-current', 'location'); else link.removeAttribute('aria-current');
    });
  }), { rootMargin: '-15% 0px -65% 0px' });
  document.querySelectorAll('.chapter').forEach(section => observer.observe(section));
}
