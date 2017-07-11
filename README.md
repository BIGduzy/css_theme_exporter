scss theme exporter
---
Replaces every instance of generic with the theme name in the generic file and writes then to a file for each theme given.

NOTE: Since ofstream can't create directories you have to make sure the dir exists.

## Example
usage: css_theme_exporter.exe path/to/generic_file path/to/output_dir theme_name ...more_theme_names

e.g.
`css_theme_exporter.exe example/generic.scss example/ light dark` for a light and a dark theme