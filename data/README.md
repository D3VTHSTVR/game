# Data Directory

This directory contains files used for inter-process communication between C++, Scheme, and Prolog.

## File Formats

### current_state.txt
Current board state written by C++, read by Scheme and Prolog.

Format:
```
<size>
<row0_values>
<row1_values>
...
<current_player>
```

Example (3x3 tic-tac-toe):
```
3
0,0,1
0,2,0
1,0,0
2
```

### move_query.txt
Move validation request written by C++, read by Prolog.

Format:
```
<row>,<col>,<player>
```

Example:
```
1,1,2
```

### move_response.txt
Response from Prolog (move validation) or Scheme (AI move).

Prolog validation response:
```
valid
```
or
```
invalid
```

Scheme AI move response:
```
<row>,<col>
```

Example:
```
0,2
```

## Notes
- Files are created/overwritten by components as needed
- Simple text format for easy debugging
- Can be inspected manually during development

