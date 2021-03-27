# Florence Engine

Florence is a game engine that can create choose your own adventure DOS games.
Games written for the Florence Engine uses a small, simple scripting language, called FlorenceScript.

## FlorenceScript

FlorenceScript is used to handle the flow of the game and to show pictures. Below is the small FlorenceScript
file that shows us the language features.

```
> exit_message: A message.

[start_point]

This is the start label, the first place player goes. New lines are preserved as is, except when preceeded with a ^
sign, for instance this won't be preserved. Neither will the new lines before you start the text.
; Comments will also not be preserved, comments must start with a semicolon and continue until the end of the line.
; This goes on until we reach the choices, which are lines starting with *.

* choice_one
* choice_two

[choice_one]

Ah, another label, if a label is the ending label, then, simply point to.

-> exit

; Exit is a global label, directions such as this just direct you to the next label.

[!choice_two]

This will directly go to choice_one after we show a picture, you can add labels with images by using the
! character.

-> choice_two 
```
