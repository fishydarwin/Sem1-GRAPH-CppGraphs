## Format for Activities

Activities are stored on three lines.\
Example:
```txt
0 3
1 2 3 
01.02.2023-10:11:12 600
```

For example, the following activity suggests:
1. Activity #0 has 3 pre-requisites.
2. Its prerequisites are Activity #1, #2, and #3.
3. The starting time is 1st of February 2023 at time 10:11:22.
4. The time to complete the activity is 600 minutes.

Therefore:
- On the first line, write the activity ID and how many prerequisites it has.
- On the second line, write the prerequisite activity IDs.
- On the third line, write the start date and time, as well as the duration to complete in minutes.
