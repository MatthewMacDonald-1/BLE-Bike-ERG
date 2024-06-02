# Workout File Format

## Manifest File

The workout manifest document (`./src/resources/workouts/Manifest.txt`) contains a list of workout filenames (`.workout`) relative to the manifest document.

## Workout File

A workout file is essentially a csv file with a different extension.

### Format

- The first line of the file is the name of the workout.
- The second line of the file is a list of headers for the rows below. The values for a column must be from the below list:
	- `time_sec` - This is an integer value that defines the length of a workout segment.
	- `target_ftp` - This is an integer value that defines the target power point of the segment. This column is not compatible with workouts that have a `target_power` column.
	- `target_power` - This is an integer value that defines the target power point of the segment. This column is not compatible with workouts that have a `target_ftp` column.
- All following lines contain values that correspond to the column values from line two. 
- The last line of the file must be an empty line.

Each workout must contain a `time_sec` column and on of `target_ftp` or `target_power`.
