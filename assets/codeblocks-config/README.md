# codeblocks-env-config

This repo contains my personal theme's configurations for Code::Blocks IDE and options for adding Allegro 5.2's libraries on it's compiler. It works on variable operational systems.

### Linux

1) **Backup original "default.conf" file by just renaming it:**

       mv /home/$USER/.config/codeblocks/default.conf /home/$USER/.config/codeblocks/BKP_default.conf

2) **Copy files to Code::Blocks IDE config folder:**

- Replace _<folder_name>_ with the folder's name you have chosen based on your OS option.

       cd <folder_name> && cp -r * /home/$USER/.config/codeblocks/
       
### Windows

1) **Open CodeBlocks, then close it, in case you haven't done it yet.**

2) **Just copy "UserTemplates" folder to "%AppData%/codeblocks".**

#
### Themes

**I did not make any of the themes contained in this repository, see the original article for more info on this <a href="https://yuchen52.medium.com/change-editor-theme-for-code-blocks-windows-linux-mac-92e9c15cbca4#:~:text=Open%20Code%3A%3ABlock.,you%20use%20a%20dark%20background.">link</a>.**

#
* Allegro's integration options on these files does not work for flatpak or snap versions of CodeBlocks, I use it only on .deb version.

* Allegro's integration options on these files requires installing Allegro 5.2's libraries in your system. If you haven't done it yet, you should get information about it on Allegro's website: https://liballeg.org/
