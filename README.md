# structdis


use
`python3 structdis.py <path/to/ctx.c> <path/to/binary.bin> offset TypeName instance_name`

example:

```
$ python3 structdis.py test/ctx.c test/bin.bin 0 ActorInfo chMadeupActor
ActorInfo chMadeupActor = { 0x3, 0x4, 0x353, 0x1, 0x80366010, 0x802c6240, 0x80326224, 0x802c6e84, { 0xc, 0x80, 0x0, 0x0}, 0.0f, { 0x0, 0x0, 0x0, 0x0}};
``` 
