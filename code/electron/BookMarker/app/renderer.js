const { shell } = require('electron')
const parser = new DOMParser()

const linksSection = document.querySelector('.links')
const errorMessage = document.querySelector('.error-message')
const newLinkForm = document.querySelector('.new-link-form')
const newLinkUrl = document.querySelector('.new-link-url')
const newLinkSubmit = document.querySelector('.new-link-submit')
const clearStorageButton = document.querySelector('.clear-storage')


const clearForm = () => {
    newLinkUrl.value = null
}

const storeLink = (title, url) => {
    localStorage.setItem(url, JSON.stringify({ title: title, url: url }))
}

const getlLinks = () => {
    return Object.keys(localStorage)
        .map(key => JSON.parse(localStorage.getItem(key)))
}

const parseResponse = (text) => {
    return parser.parseFromString(text, 'text/html')
}

const convertToElement = (link) => {
    return `
    <div class="link">
    <h3>${link.title}</h3>
    <p><a href="${link.url}">${link.url}</a></p>
    </div>
    `
}

const renderLinks = () => {
    const linkElements = getlLinks().map(convertToElement).join('')
    linksSection.innerHTML = linkElements
}

const findTitle = (nodes) => {
    return nodes.querySelector('title').innerText
}

const handleError = (error, url) => {
    errorMessage.innerHTML = `failed when adding "${url}": ${error.message}`.trim()
    setTimeout(() => errorMessage.innerText = null, 5000)
}

newLinkUrl.addEventListener('keyup', () => {
    // console.log('newLinkUrl keyup event')
    newLinkSubmit.disabled = !newLinkUrl.validity.valid
})

newLinkForm.addEventListener('submit', (event) => {
    event.preventDefault()

    const url = newLinkUrl.value
    fetch(url)
        .then(response => response.text())
        .then(parseResponse)
        .then(findTitle)
        .then(title => storeLink(title, url))
        .then(clearForm)
        .then(renderLinks)
        .catch(error => handleError(error, url))
})

linksSection.addEventListener('click', (event) => {
    if(event.target.href) {
        event.preventDefault()
        shell.openExternal(event.target.href)
    }
})

clearStorageButton.addEventListener('click', () => {
    localStorage.clear()
    linksSection.innerHTML = ''
})

renderLinks()