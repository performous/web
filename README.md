# Performous.org

The website is statically generated from custom format, using the tool in webconv folder.

Static assets are in htdocs-binary and source code in `htdocs-source`, and in particular the navigation menu is defined in `htdocs-source/AWC-cfg.txt`. See `webconv/README.txt` for mode details on the source formats and the conversion tool.

Use `local-deploy.sh` script to rebuild into `web` folder, that can then be uploaded to web server.
