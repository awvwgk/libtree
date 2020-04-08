#include <bundler/deploy.hpp>
#include <bundler/exec.hpp>

#include <termcolor/termcolor.hpp>

// Copy binaries over, change their rpath if they have it, and strip them
void deploy(std::vector<Elf> const &deps, fs::path const &bin, fs::path const &lib, fs::path const &strip, fs::path const &chrpath) {
    for (auto const &elf : deps) {

        // Go through all symlinks.
        auto canonical = fs::canonical(elf.abs_path);

        //Copy to the deploy folder
        auto deploy_folder = (elf.type == deploy_t::EXECUTABLE ? bin : lib);
        auto deploy_path = deploy_folder / canonical.filename();
        fs::copy_file(canonical, deploy_path, fs::copy_options::overwrite_existing);

        std::cout << termcolor::green << canonical << termcolor::reset << " => " << termcolor::green << deploy_path << termcolor::reset << '\n';
        // Create all symlinks
        for (auto link = elf.abs_path; fs::is_symlink(link); link = fs::read_symlink(link)) {
            auto link_destination = deploy_folder / link.filename();
            fs::remove(link_destination);
            fs::create_symlink(deploy_path.filename(), link_destination);

            std::cout << "  " << termcolor::yellow << "creating symlink " << link_destination << '\n';
        }

        auto rpath = (elf.type == deploy_t::EXECUTABLE ? "\\$ORIGIN/../lib" : "\\$ORIGIN");

		// Silently patch the rpath and strip things; let's not care if it fails.
        std::stringstream chrpath_cmd;
        chrpath_cmd << chrpath << " -c -r \"" << rpath << "\" " << deploy_path;
        exec(chrpath_cmd.str().c_str());

        std::stringstream strip_cmd;
        strip_cmd << strip << ' ' << deploy_path;
        exec(strip_cmd.str().c_str());
    }
}