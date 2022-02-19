# prinit
Prinit is a tool for windows to start your projects faster.

# command

>**If you want to use prinit you'll probably want to add it to the _path_**

- Download last release of prinit
- Copy prinit folder and paste it in C:\
- Add C:\prinit to your computer path like this https://stackoverflow.com/questions/44272416/how-to-add-a-folder-to-path-environment-variable-in-windows-10-with-screenshots

You can use prinit like this :

`prinit [project_name] [project_workspace] [project_type]`

Prinit will create a folder called *project_name* at folder path *project_workspace* with files of template folder *project_type*.

If you do :

`prinit my_project C:\users\[username]\Desktop cpp`

You will have a folder called my_project on your desktop with the main files for a cpp project.

# settings

With *setting.txt* you can change the command order. `[project_name] [project_workspace] [project_type]`

With *workspaces.txt* you can create a shortcut for a folder path that you use frequently.

# workspaces

Add this line in *workspaces.txt* :

`Desktop : C:\Users\[your_username]\Desktop`

And simply run the command :

`prinit my_project Desktop cpp`

And you will have the same result as before!

_**There is a template folder with subfolders, these subfolders are the project_types, you can modify them as you please and/or add new ones.**_

# templates

_templates_ is the folder where templates folders are stored (cpp, c, etc...).

- You can rename these folders as you please
- You can modify their content (add files, modify files, etc...)
- You can create new folders

Everything is done automatically !

# Options

Options are stored in folders named like _!prinit-options_

There is one global option folder stored in the root of prinit fodler
and you can create template-options in template's folders

Options will modify your template by merging the folders

Non existing files will be added in your project files
Existing files will be replaced (Files will be searched recursively, the first one found will be replaced)

Template options can be specified with a '-' like "-option1"
Global options can be specified with a '.' like ".option2"

