#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdio.h>
#include <map>

// For settings files
std::string comment_beg = "//";
// Command args
std::string project_name = "";
std::string project_type = "";
std::string project_workspace = "";
// Path of the executable
std::string executable_path = "";

int b_length(char* buffer) {
    int length = 0;
    while (buffer[length] != '\0') {
        length++;
    }
    return length;
}

void b_append(char* buff_target, char* buff_copy) {
    int buff_copy_curs = 0;
    int buff_target_curs = b_length(buff_target);
    while (buff_copy[buff_copy_curs] != '\0') {
        buff_target[buff_target_curs] = buff_copy[buff_copy_curs];
        buff_target_curs += 1;
        buff_copy_curs += 1;
    }
}

void str_remove(std::string* str, int amount_to_remove) {
    if (str->length() >= (unsigned long long)amount_to_remove) {
        str->resize(str->length() - (amount_to_remove + 1));
    }
}

std::string get_execpath(char* executable_name) {
   char buffer[1024];
   char command_buffer[1024] = "where ";
   b_append(command_buffer, executable_name);
   std::string result = "";

   FILE* pipe = popen(command_buffer, "r");
   if (!pipe) {
      std::cout << "An error has occured" << '\n';
      throw "Failed to execute command";
   }

   result = fgets(buffer, 1024, pipe);

   std::string file_exec_name = executable_name;
   file_exec_name += ".exe";
   str_remove(&result, file_exec_name.length());

   pclose(pipe);
   return result;
}

bool str_beginw(std::string str, std::string beg) {
    // Return true if str begins with beg, else otherwise
    if (beg.length() > str.length()) { return false; };
    for (int i = 0; (unsigned long long)i < beg.length(); i++) {
        if (beg[i] != str[i]) {
            return false;
        }
    }
    return true;
}

bool str_endsw(std::string str, std::string end) {
    // Return true if str ends with end, else otherwise
    if (end.length() > str.length()) { return false; };
    int i = str.length() - 1; // str cursor
    int j = end.length() - 1; // end cursor
    for (j = j; j >= 0; j--) {
        if (str[i] != end[j]) {
            return false;
        }
        i--;
    }
    return true;
}

void init_settings() {
    std::ifstream file;
    std::string settings_path = executable_path;
    settings_path += "settings.txt";
    file.open(settings_path);
    if (file.is_open()) {
        // Do something with the settings
    } else {
        std::cout << "Warning : " << "Missing settings.txt file" << '\n';
        // throw "Missing settings.txt file";
    }
}

int init_type() {
    // path of the wanted template folder
    std::string tfolder_path = executable_path + "templates\\" + project_type;
    if (!std::filesystem::exists(tfolder_path)) {
        std::cout << "Type template do not exists" << '\n';
        return 0;
    }
    return 1;
}

int init_files(std::string projectf_p) {
    // if a folder with project's name exists in workspace
    // path then ask user if he wants to overwrite it
    // else create it
    if (!std::filesystem::exists(projectf_p)) {
        // copy files into the directory
        std::string template_path = executable_path + "templates\\" + project_type;
        std::filesystem::copy(template_path, projectf_p, std::filesystem::copy_options::recursive);
        return 1;
    } else {
        std::cout << projectf_p << " already exists, do you want to delete it ? (Y/(other key for NO)) > ";
        char user_choice;
        std::cin >> user_choice;
        if (user_choice == 'Y') {
            std::filesystem::remove_all(projectf_p);
            // copy files into the directory
            std::string template_path = executable_path + "templates\\" + project_type;
            std::filesystem::copy(template_path, projectf_p, std::filesystem::copy_options::recursive);
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

int init_workspace() {
    // check is workspace mentionned is a real directory
    // workspace is not a real directory => search for the 
    //      real path variable in workspaces.txt
    // workspace is a real directory => use path
    if (!std::filesystem::exists(project_workspace)) {
        std::ifstream file;
        std::string workspaces_path = executable_path;
        workspaces_path += "workspaces.txt";
        file.open(workspaces_path);
        if (file.is_open()) {
            std::string file_line;
            std::map<std::string, std::string> workspaces;
            // search for workspaces mentions in workspaces.txt
            while (file.good()) {
                std::getline(file, file_line);
                // check if line is not a comment and not empty line
                if (!str_beginw(file_line, comment_beg) && file_line.length() != 0) {
                    // workspace name - workspace path
                    std::string workspc_n = "";
                    std::string workspc_p = "";
                    int i = 0;
                    while (file_line[i] != ' ') {
                        workspc_n += file_line[i];
                        i++;
                    }
                    // skip line separator
                    // "workspace_name : workspace_path"
                    //               |---^   
                    //                   i + 3
                    i += 3;
                    while (file_line[i] != '\0') {
                        workspc_p += file_line[i];
                        i++;
                    }
                    workspaces[workspc_n] = workspc_p;
                }
            }
            // check if workspace is registered
            if (!workspaces[project_workspace].length() == 0) {
                // project folder path
                std::string projectf_p = "";
                if (str_endsw(workspaces[project_workspace], "\\")) {
                    projectf_p = workspaces[project_workspace] + project_name;
                } else {
                    projectf_p = workspaces[project_workspace] + "\\" + project_name;
                }
                // return 1 if everyting gone well 0 otherwise
                return init_files(projectf_p);
            } else {
                return 0;
            }
        }
    } else {
        // project folder path
        std::string projectf_p = "";
        if (str_endsw(project_workspace, "\\")) {
            projectf_p = project_workspace + project_name;
        } else {
            projectf_p = project_workspace + "\\" + project_name;
        }
        // return 1 if everyting gone well 0 otherwise
        return init_files(projectf_p);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        init_settings();
        // argv[0] is the name of the executable
        executable_path = get_execpath(argv[0]);
        // Store args
        project_name = argv[1];
        project_type = argv[2];
        project_workspace = argv[3];
        // itype_s = 1 if type is valid 0 otherwise
        int itype_s = init_type();
        if (itype_s) {
            int iworkspc_s = init_workspace();
            if (iworkspc_s) {
                std::cout << "Project succesfully created" << '\n';
            }else {
                std::cout << "An error has occured" << '\n';
            }
        }
    } else {
        std::cout << "Yout need to specify every parameters, see in settings.txt" << '\n';
    }
    return 0;
}
