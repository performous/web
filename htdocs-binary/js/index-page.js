let img = Math.floor(Math.random() * 20)
let anim = -1
const framesAnim = 120
const framesImg = 400
let lastTimestamp

function showcaseScroll(timestamp) {
  if (lastTimestamp === undefined) lastTimestamp = timestamp
  const deltaTime = timestamp - lastTimestamp
  lastTimestamp = timestamp
  const animDelta = (deltaTime * 60) / 1000
  anim += animDelta
  if (anim >= framesImg) {
    anim = 0
    ++img
  }
  const a = Math.min(anim / framesAnim, 1.0)
  const pos = 5 * 128 * (img + 0.5 * (1 - Math.cos(Math.PI * a)))
  const showcase = document.getElementById("showcase")
  showcase.style.backgroundPosition = Math.round(-pos) + "px 0"
  requestAnimationFrame(showcaseScroll)
}
requestAnimationFrame(showcaseScroll)

function smartDownload() {
  const e = document.getElementById("download")
  if (!e) return
  const platform = navigator.userAgentData.platform
  if (platform === "Windows") {
    // Windows
    e.href = "https://github.com/performous/performous/releases/download/1.3.1/Performous-1.3.1-msvc.exe"
    e.title = "Performous 1.3.1 (Windows)"
  } else if (platform === "Linux") {
    // Linux
  } else if (platform === "macOS") {
    // Mac/Mac OS X
    e.href = "https://github.com/performous/performous/releases/download/1.3.1/Performous-1.3.1.dmg"
    e.title = "Performous 1.3.1 (OS X 10.6 or newer)"
  } else {
    // Other
  }
}

// Poll for updates (if user navigates to index page from another page)
setInterval(smartDownload, 50)
