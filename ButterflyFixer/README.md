# ButterflyFixer
Workaround for macbook pro 2016+ double key pressing problem

#### This is NOT a FIX. Problem is butterfly2 keyboard itself (hardware). 
#### This is a WORKAROUND. 

#### How it works.
It checks if interval between pressing SAME button is very small (100ms by default) and canceling second keypress.

### Why 100ms? 
1) The fastest button pressing I tried - near 150ms, so app won't block valid user input. 
2) All double pressing I catched are under 100ms. 

You could increase inverval if app don't block some double keypresses.

### How to setup:

1) Open `config.plist` file. 
2) Think good about timeout, edit if needed (`timeout` key)
3) Write key codes of buttons you want to filter. (`blacklisted_keys` key). 
Edit carefully, this is array of numbers. You can remove default keys (`c` and `enter`).

### Where to get codes:

install https://itunes.apple.com/tr/app/key-codes/id414568915?l=tr&mt=12 , press button and look at `Key Code`. E.g. for enter it's 36.

Other way, go to https://stackoverflow.com/questions/3202629/where-can-i-find-a-list-of-mac-virtual-key-codes , find and convert HEX to dec.

### How to install:

1) clone or download this repo
2) open terminal and go to project directory (cd ~/Downloads/ButterflyFixer)
3) run `bash install.bash`
4) install developer tools if needed (you will be prompted)
5) enter admin password
6) ready. You can check status and logs by `bash status.bash` and `bash logs.bash`

### Why you prompted to enter admin password? 
`Quartz Event` api requires admin privileges to read keyboard input, so no way to avoid this.

### Should I worry about privacy and security?
No. No binaries are downloaded, app is building from sources. So everything is transparent.

### How to uninstall:

`bash clean.bash`


#### PS. For developers who know Cocoa and have developer certificate. You can help with writing Perf pane and privileged helper tool. (which requires signing). PRs are welcome.
