# Performous Website

The website is statically generated from custom format, using the tool in `webconv` folder.

Static assets are in `htdocs-binary` and source code in `htdocs-source`, and in particular the navigation menu is defined in `htdocs-source/AWC-cfg.txt`. See `webconv/README.txt` for mode details on the source formats and the conversion tool.

Use `local-deploy.sh` script to rebuild into `web` folder.

# Github Actions

We're using Github Actions to automaticly deploy the website.

To update the website follow these actions:

1. Make your changes to the website
2. Propose your changes in a Pull Request
3. Github Actions will produce an artifact which we can use to verify the changes
4. Fix any feedback that might come and fix it until an approval is given
5. Rebase and Merge to master
6. GH-Actions will auto-deploy the changes.