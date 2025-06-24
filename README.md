Smart Gnome Control, The Return...

Smart Gnome Control is a graphical interface is a graphical user interface to multiple communications receivers via the [Hamlib library](https://github.com/Hamlib/Hamlib). The specific purpose is to let you control your communications receiver from a personal computer, and to simplify the hobby of shortwave radio listening.

The Smart Gnome Control interface is designed to automatically reconfigure according to the known capabilities of your receiver. It is also designed to provide database-driven tuning, memory and logging capabilities, based on FineWare's (discontinued) Smart Control series of receiver control packages for Windows, and as updated with the database capabilities of Frequency Browser. The Smart Gnome Control interface is written for Linux in GNU C, using Glade, Gdk/Gtk+
and curl. It also requires version 4.6 or later of Hamlib. With a little help from [Homebrew](https://brew.sh/) libraries, Smart Gnome Control should also be able to be built on macOS. 

Smart Gnome Control began as an idea I had 21 years ago under Gnome v1,2. It was to be the Linux version of what was Smart R8 Control v4 at the time, which also explains the application icon. It hasn't been updated in 20 years. So if you don't get the pun, `The Return...` is an homage to David Lynch and Twin Peaks.

# Dependencies

The following are required for Smart Gnome Control:

* [Hamlib 4.6](https://github.com/Hamlib/Hamlib)
* curl 8.7
* Gtk+ 3.24
* Gdk 3.24
* GLib 2.84
* A computer-controllable HF receiver or transceiver

It is assumed you have a fully operational Gtk3 environment installed from [Homebrew](https://brew.sh/) if running on macOS. If not, please see the Special notes regarding Homebrew, below.

# Setup under Linux and macOS

1. Ensure you have the requisite Gtk3/Gdk3/Glib2 and curl packages installed in Linux by querying dnf and or rpm.
2. Installing Hamlib. You can also check if the latest full release of Hamlib is installed this way. If you prefer to download the latest source tree from Github and build it fresh, you can do so by moving to your home directory where you build software (e.g., `~/Development`) and typing `git clone https://github.com/Hamlib/Hamlib.git hamlib`. This will create a directory named hamlib there and pull the latest development tree into it. Once this is complete, go to the top of the hamlib directory (e.g. `~/Development/hamlib`) and type the following commands to build it:
```
./bootstrap
./configure
make
sudo make install
sudo ldconfig [Note: only needed under Linux, not macOS]
```
3. Installing Smart Gnome Control. Next, move to the top Smart Gnome Control directory (e.g., `~/Development/sgcontrol`) and type the following to build it:
```
./autogen.sh
./configure
make
sudo make install
sudo ldconfig [Note: only needed under Linux, not macOS]
```
4. Running Smart Gnome Control. If all goes well, you should be able to start Smart Gnome Control with the `sgcontrol` command in a terminal, or from the icon under Apps -> Other.

## Special notes and instructions for building under Linux or macOS
1. configure. Note that configure allows you to alter the prefix for where things install. The default is usually `/usr/local`. You can type `./configure --help` to provide options on how to change this.
2. make. Also note that if your computer uses a multi-core processor, using `make -jx`, where `x` is the number of possible cores, will drastically speed up the build process, especially for Hamlib, which is massive.
3. ldconfig. ldconfig is completely unnecessary for macOS. Unsure if this is still the case with some Linux distros, but at one time there was a weird issue where it didn't actually update pkconfig paths unless the `-v` switch was used, making it impossible for software packages to find and build against it. If you get build errors for Smart Gnome Control telling you it can't find Hamlib, go badk to the Hamlib directory and issue `sudo ldconfig -v`. Once bitten, twice shy, and I always use the `-v` switch as a reflex action.

## Special notes and instructions for building and running under macOS
Requisite Homebrew packages to emulate Linux on Mac: As previously mentioned, it expectedt that you already have a fully operational Gtk3 environment installed and working on your Mac. If not, you will need (at a minimum) the following Homebrew packages: adwaita-icon-theme, atk, autoconf, autogen, automake, cairo, gcc, gdk-pixbuf, gettext, git, glib, gnome-common, gnome-icon-theme-alias, gnome-themes-extra, gnome-themes-standard, gnu-sed, gnutils, gobject-introspection, gsettings-desktop-schemas, gtk+, gtk+3, gtk-doc, gtk-mac-integration, gtkglext, guile, harfbuzz, gtksourceview3, hicolor-icon-there, intltool, itstool, libevent, libglade, libgnomecanvas, libnotify, libpthread-stubs, libtool, libusb, libusb-compat, libx11, libxau, libxcb, libxdmcp, libxext, libxfixes, libxi, libxml2, libxrender, libxtst, m4, pango, pkgconf, and xorgproto. This list is not all inclusive as it represents a quick glance and an educated guess from what I already have installed. Building errors will likely give you a hint to what I may have left out.

Once this is done, you may need to also alter your PKG_CONFIG_PATH environment variable to include Homebrew at the beginning: `export PKG_CONFIG_PATH=<homebrew prefix>/Library/Homebrew/os/mac/pkgconfig/15:<homebrew prefix>/lib/pkgconfig:<homebrew prefix>/share/pkgconfig:$PKG_CONFIG_PATH`. It may also be helpful to also add the following to your PATH environment variable: `export PATH=<homebrew prefix>/sbin:<homebrew prefix>/bin:$PATH`. In each case, replace `<homebrew prefix>` with your particular prefix as it's different for Intel and ARM-based Macs (/usr/local/homebrew/ vs. /opt/homebrew/). These can be made permanent by adding each of those `export` lines to your particular shell profile file (e.g., `~/.zprofile` for zshell, or `~/bash_profile` for bash).

Homebrew may provide you other caveats to employ after installing the above packages. For example, many of them are installed inertly so they don't clash with things that macOS has already provided, but the Homebrew install provides a path to build against valid includes and libraries that Apple didn't provide. For example, I override macOS's version of guile. As a result, I also have to add `export GUILE_LOAD_PATH="opt/homebrew/share/guile/site/3.0"`, `export GUILE_LOAD_COMPILED_PATH="/opt/homebrew/lib/guile/3.0/site-ccache"`, and `export GUILE_SYSTEM_EXTENSIONS_PATH="/opt/homebrew/lib/guile/3.0/extensions"` to my `.zprofile`, as directed by Homebrew. Standard Disclaimer: Don't override anything in macOS if you are not comfortable doing so.

GSettings errors: There is a possibility that you'll get a critical error when running `sgcontrol` from a terminal the first time. For some reason, Homebrew's version of glib under macOS doesn't like it sitting under the `/usr/local` prefix. If so, this may fix it:
1. Fnd the file `org.markjfine.sgcontrol.gschema.xml` in the top of the Smart Gnome Control source tree and copy it to `~/.local/share/glib-2.0/schemas` (e.g., `cp org.markjfine.sgcontrol.gschema.xml ~/.local/share/glib-2.0/schemas`).
2. Run `glib-compile-schemas ~/.local/share/glib-2.0/schemas` to install it.

Outboard USB serial ports: If you are using a USB-based PL2303-type serial port, there is a more than likely chance that macOS won't automatically map it to something easy, like `/dev/ttyUSB0`. In this case, you will have to choose `Other...` in the list of serial ports and type something like `/dev/tty.PL2303G-USBtoUART11440` in the box next to it. You will have to run something like `ls -la /dev/tty*` to figure out exactly which device to use.

Other known issues: Know that there are some other known quirks that are specific to running under macOS that still need to be worked out:
1. The first thing is that the faceplate window doesn't want to resize properly. When opening and closing the faceplate sidebars such as the Control Panel, the faceplate should shrink back to size. It doesn't under macOS.
2. Every one of the lists and tables sort in a descending order, rather than ascending, as specified (And works) under Linux. This is most annoying with the Station sidebar in the database display, because there's no heading to click on to re-sort it.

# Usage
Some receivers like to be on before Smart Gnome Control is run. If you have a receiver that is capable of working at multiple serial rates, ensure it is set at the highest rate. For example, my Icom R-75 will not connect to Hamlib unless it is set at 19200. If needed, I have an old list of the serial pinouts for cabling a Drake R8/R8A/R8B, JRC NRD-353D/545, Icom R75, and Kenwood R5000 at [http://fineware-swl.com/faq/](http://fineware-swl.com/faq/).

## Settings
The first thing the application may do is to download a fresh version of the [Eibi shortwave database](http://eibispace.de/). It will check this and automatically install a new version of the database (if available) each time you start Smart Gnome Control.
You may select the radio you will be using by selecting Tools->Options or by clicking on the gear button towards the right of the RadioCtrl toolbar.
The first tab of Options shows the different radios Smart Gnome Control will operate (via Hamlib). Select a radio, and the appropriate port below. We've included most of the standard serial devices, but if it's not there choose `Other...` and type the device path on the right. Clicking `Ok` or `Apply` will reconfigure your display and immediately attempt to connect to your radio.
The second tab of Options shows various application settings, which will expand over time. Currently, there are two options:
1. An option to filter out AM broadcast stations from the database sidebar.
2. An option to toggle automatic database updates.
3. There will be more as application development continues.

## Application Operation
Most of the controls are fairly self-explanatory. Those that are not self-explanatory will generate prompts in the status bar at the bottom.

### Direct Tuning
You can tune each digit in the frequency display individually: Clicking a digit with the left button decreases the number, the right button increases it. You may also type in a frequency and hit enter; or use the keypad to the right of the display. Below the keypad is a tuning bar that allows you to do large and small tuning steps. Right-clicking on one of these buttons will bring up a small menu that will allow you to change the large or small tuning step. You may also tune up and down in large increments using the left and right arrow keys on the keyboard. Holding down the Ctrl key will use the small increments.

Smart Gnome Control also includes a tuning history. You can use the left and right arrow buttons at the left of the RadioCtl toolbar to navigate this history. Choosing View->History or typing Alt-H will open the History sidebar allowing you navigate your timeline, much like a web browser. The frequency display will show the current band to the right of the signal meter. Clicking on this, selecting View->Band Plan, or pressing the F2 key will bring up the Band Plan sidebar. Clicking on any frequency in the band plan will tune you to that particular frequency, allow you to start tuning through a particular band. 

At the bottom of the faceplate, you will have standard controls to set the antenna, VFO, AGC, bandwidth, and mode. Some of these might not be available for your radio and will be disabled. The RadioCtrl toolbar will at best have up to two noise blankers, dynamic noise reduction, multi-functional attenuator and preamp buttons, as well as a notch or automatic notch function. If your radio has any level-type controls, those will be in the Control Panel sidebar. The Control Panel can be unhidden from the View->Control Panel menu item or by typing Alt-P.

### Memory Tuning
You can switch the radio to memory mode from the Tuning->Memory mode menu item, typing Ctrl-E, or by clicking the toolbar button that looks like a clipboard. While in memory mode, MEM will be displayed to the left of the current memory channel. While in memory mode, the left-right tuning will operate on memory channels. You can bring up a list of memories by choosing View->Memories, typing Ctrl-H, clicking the button that looks like a lined clipboard, or by simply clicking on the memory channel indicator.

It order to make this valuable, you should first Capture the radio's memories into a default memory file. Each radio model will have it's own default memory file, which should be treated as a mirror of what's in the radio. Editing a default memory file in the memory display will automatically update that channel if the radio is on. You can backup the current default memory file after a Capture, by selecting Save As... and entering a simple name for it. If you want to return to the default set of memories, you can choose Open.... You may also create sets of memory files by creating a New memory file, entering a name, then simply editing it. There's no need to save it because all edits are automatically added to the file. Once the file is completed, you may Fill the radio with it (making it the new default). Editing any memory file is done simply by clicking the cell you want changed, similar to a spreadsheet.

### Database Tuning
Smart Gnome Control's makes use of the Eibi shortwave schedules. Whereas the older Smart Control applications used six different windows to show information, Smart Gnome Control leverages the Frequency Browser layout to provide a fully integrated display. While tuning, Smart Gnome Control will automatically display all stations that are within 5kHz of the tuned frequency right on the faceplate. You can navigate the rest of the database through the integrated display by choosing View->Tuning database, typing the F5 key, or by clicking the button that looks like an eye in the Database toolbar.

Using the Tuning Database is fairly straight-forward: The left hand-side shows all stations currently on. Each will be ordered according to one of the following color-coded categories: Broadcast, Broadcast Digital, Utility Aircraft, Utility Digital, Utility Fixed/Land, Utility Marine, and Unknown (uncategorized). The Stations sidebar can be further filtered by Language, Country (e.g., everything coming from France), Region (e.g., everything coming from Africa), or by Target (e.g., everything beamed to Europe).

Once you have the filters set, you can choose a station name to find it's parallel frequencies. The default is to display frequencies being currently used by that station, but you can remove that time filter by clicking the clock button to the right of Station View. Clicking on any one of the Station View records will display everything on now within 5kHz in Frequency View section, above it. Similarly, clicking on any of the listings in the Frequency View will automatically show any parallel frequencies being used by that station in the Station View. You may also unselect the time filter on the Frequency View in order to show usage about that frequency all day. The listings at the bottom of the faceplate is essentially an extended Frequency View display, so left-clicking on a particular item will display the station's parallel frequencies in the Station View. 

### Logging Operation

Smart Gnome Control integrates a full-fledged logging facility. Like the memories, all logging is entered into the file, in real time so there's no real need to remember to save anything. The log display can be shown by selecting the View->Log display menu item, pressing the F4 key, by clicking on the RadioCtrl toolbar button that looks someone writing on a document. The log display will automatically show with the most recent entry highlighted. To add a new entry, click the Clear button (if necessary), enter the date, time, frequency, call, station, country, mode, and SINPO, as well as any free-form details in the Notes below, then click the Add button. You can take a shortcut, by right clicking on the station you want to log in the faceplate.  This will automatically fill most of this information, as well as adding the language and target in the Notes. Clicking on any entry in the log listing will auto-fill the editing area. You may choose to alter any of it and click the Modify button to change the entry, of Delete to delete it.

## Screenshots
![radio faceplate](https://raw.githubusercontent.com/markjfine/sgcontrol/master/screenshots/faceplate.png "Radio Faceplate")
![options](https://raw.githubusercontent.com/markjfine/sgcontrol/master/screenshots/options.png "Options")
![memories](https://raw.githubusercontent.com/markjfine/sgcontrol/master/screenshots/memories.png "Memories")
![database](https://raw.githubusercontent.com/markjfine/sgcontrol/master/screenshots/database.png "Database")
![logging](https://raw.githubusercontent.com/markjfine/sgcontrol/master/screenshots/logging.png "Logging")

## Version History
0.3 Initial Release in 2004 (yeah, I know)
0.5 Updated to Gtk3 and completely modernized the application 
