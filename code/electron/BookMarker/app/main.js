const { app, BrowserWindow } = require('electron')

let mainWindow = null

app.on('ready', () => {
    console.log('Hello from Electron')
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: true
        }
    })
    mainWindow.webContents.loadURL(`file://${__dirname}/index.html`)
    mainWindow.webContents.openDevTools()
})