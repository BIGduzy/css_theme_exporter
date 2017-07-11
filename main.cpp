#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief prints help text to console
 */
void printHelp() {
    std::cout << "usage: css_theme_exporter.exe path/to/generic_file path/to/output_dir theme_name ...more_theme_names\n";
}

/**
 * @brief Asks user if the program should continue
 *
 * @param genericFile path/to/generic_file
 * @param outputDir path/to/output_dir
 * @param themes vector with all the theme names
 * @return if the program should continue
 *
 * @details
 * Prints the given parameters to console and asks the user if the data is correct
 */
bool goNoGo(const std::string& genericFile, const std::string& outputDir, const std::vector<std::string>& themes) {
    std::cout << "\nPlease check the following: \n\n";

    // Print the variables
    std::cout << "Generic file: " << genericFile << '\n';
    std::cout << "Output folder: " << outputDir << '\n';

    size_t themeCount = themes.size();
    std::cout << "Themes(" << themeCount << "): \n";
    for (uint8_t i = 0; i < themeCount; ++i) {
        std::cout << "\t- " << themes[i] << '\n';
    }

    std::cout << "\nLooking good (Y/N)?\n";
    char answer;
    std::cin >> answer;

    return (answer == 'Y' || answer == 'y');

}

/**
 * @brief Reads a file, finds and replaces words and then outputs to the outputFile
 * @todo Currently we open and close the generic file for every theme, this should only be done once
 *
 * @param genericFile path/to/generic_file
 * @param outputFile path/to/output_file
 * @param wordFind word that needs to be replaced
 * @param wordReplace word to replace with
 * @return if successfully replaced
 */
bool replaceInFile(const std::string& genericFile, const std::string& outputFile, const std::string& wordFind, const std::string& wordReplace) {
    std::ifstream in(genericFile);
    std::ofstream out(outputFile);

    if (!in) {
        std::cerr << "Could not open " << genericFile << "\n";
        return 0;
    }

    if (!out) {
        // NOTE: Can't create directory if not exits :(
        // https://stackoverflow.com/questions/18682148/how-to-create-directories-automatically-using-ofstream
        std::cerr << "Could not open " << outputFile << " make sure the directory exits" << "\n";
        return 0;
    }

    std::string line;
    size_t len = wordFind.length();
    // For each line
    while (getline(in, line)) {
        while (true) {
            size_t pos = line.find(wordFind);
            // Could not find word
            if (pos == std::string::npos) break;

            // Replace word
            line.replace(pos, len, wordReplace);
        }

        // Write to file
        out << line << '\n';
    }

    // Close files
    in.close();
    out.close();

    return 1;
}

int main(int argc, char* argv[]) {
    // No arguments given
    if (argc == 1) {
        std::cout << "Missing arguments, use the help commando for more information";
        return 1;
    }

    // Help command
    std::string genericFile = argv[1];
    if (argc > 1 && genericFile.compare("help") == 0) {
        printHelp();
        return 0;
    // Missing arguments
    } else if (argc < 4){
        std::cout << "Missing " << (4 - argc) << " arguments, use the help commando for more information";
        return 1;
    }

    // Put the rest of the program arguments in variables
    std::string outputDir = argv[2];
    std::vector<std::string> themes = { argv[3] };
    // Add the other themes, if there are any
    for (uint8_t i = 4; i < argc; ++i) {
        themes.push_back(std::string(argv[i]));
    }

    // Abort program if user wants
    if (!goNoGo(genericFile, outputDir, themes)) {
        std::cout << "Program aborted\n";
        return 0;
    }

    /*
     * Let's go!
     */
    std::cout << "\n\nLet's go then!\n";

    // Find location of first dot
    size_t dotLocation = genericFile.find_last_of('.');
    size_t lastSlashLocation = genericFile.find_last_of('/') + 1; // + 1 because we don't want the '\' in the wordFind

    // Get file extension
    std::string fileExtension = genericFile;
    fileExtension.erase(0, dotLocation); // Erase every thing till before the last .

    // Find the word we want to replace by creating a substring between last \ and last .
    std::string wordFind = genericFile.substr(lastSlashLocation, dotLocation - lastSlashLocation);

    // Create the theme file for every theme
    size_t themeCount = themes.size();
    for (uint8_t i = 0; i < themeCount; ++i) {
        // Create name of output file
        std::string outputFile = outputDir + "/" + themes[i] + fileExtension;

        // Replace word and write to the output file
        if(replaceInFile(genericFile, outputFile, wordFind, themes[i])) {
            std::cout << "\t- " << themes[i] << " done\n";
        } else {
            std::cout << "Oops!, something went wrong with theme: " << themes[i] << " :(\n\n";
        }
    }

    std::cout << "All themes are done :)\n";
    return 0;
}

