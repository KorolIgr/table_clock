// nav.js
fetch('nav.html')
  .then(res => res.text())
  .then(html => document.getElementById('nav').innerHTML = html);