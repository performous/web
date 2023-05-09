// Store the current page URL to track navigation
let currentPage = location.href
const title = document.querySelector("title")
const page = document.querySelector("#page")

// Fetch another page when user navigates within the site
async function switchPage() {
  if (location.href === currentPage) return false
  // Update the current page
  currentPage = location.href
  try {
    // Fetch the page and extract the new content (title and #page)
    const response = await fetch(currentPage)
    const data = await response.text()
    const parser = new DOMParser()
    const newData = parser.parseFromString(data, "text/html")
    const newTitle = newData.querySelector("title")
    const newContent = newData.querySelector("#page")
    // Update the page content
    if (newContent) {
      title.innerHTML = newTitle.innerHTML
      page.innerHTML = newContent.innerHTML
    }
  } catch (error) {
    // If there's an error, fallback to the old-fashioned way
    location.href = currentPage
  }
}

// Add click event listener to handle internal links
document.addEventListener("click", async (ev) => {
  const target = ev.target.closest("a")
  // Ignore external links
  if (!target || target.hostname !== location.hostname) return
  // Prevent default navigation, we'll handle it ourselves
  ev.preventDefault()
  history.pushState(null, "xhr", target.href)
  await switchPage()
})

// Add a popstate event listener to handle back/forward navigation
addEventListener("popstate", switchPage)
