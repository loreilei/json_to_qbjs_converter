This tool reads JSON files and output files encoded in Qt5's internal binary json format.

`qbjs` stands for **Q**t5's **B**inary **Js**on.

## Build

Provided you have common build tools installed (gcc, make, cmake) and Qt5.15 installed on your machine.

```bash
# From repository's root
mkdir build && cd build
cmake ../src
make
```

## Example usage

This tool take 2 arguments:
- a path to the folder containing the JSON files to convert
- a path where the converted files should be written

For example, the JSON file `fileA.json` would be converted to `fileA.qbjs`.

Considering the following folder structure:
- JSON files to convert are located in the folder `/home/user/json_files`
- converted files should be located in the folder `/home/user/qbjs_files`

The corresponding call would be (on several lines for readability):

```bash
./json_to_qbjs_converter \
--json_files_folder /home/user/json_files \
--qbjs_files_folder /home/user/qbjs_files
```
