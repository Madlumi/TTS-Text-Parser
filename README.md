# stupid tts thing
quick (and very dirty) handler for running festival tts (becasue festival was being mean to me).

config by editing main.c

make is just a gcc command

command to speak selection:
xclip -out -selection primary |/path/to/ttsc
