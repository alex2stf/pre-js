sten-js
=========
### Simple Template Engine

`sten-js` is a basic easy-to-use template engine for nodejs.<br/>
All code placed inside `<?stn` and `stn?>` tags will be compiled into a separate javascript context.


How to Use
----------

```javascript
var sten = require("../index.js");
var str = "hi this is some random <?stn var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'));  stn?> to parse";
sten.createContext();
sten.parse(str, function(msg) {
    console.log(msg);
});
```

Some additional examples include `cin()` method:

 * [Test addon example](https://github.com/alex2stf/sten-js/blob/master/test/testaddon.js)

Notes
-----

**Tested with the following node versions:**
- 0.10.28


License
-------

(The WTFPL License)

  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.


[windows-python]: http://www.python.org/getit/windows
[windows-python-v2.7.3]: http://www.python.org/download/releases/2.7.3#download
[msvc2010]: http://go.microsoft.com/?linkid=9709949
[msvc2012]: http://go.microsoft.com/?linkid=9816758
[win7sdk]: http://www.microsoft.com/en-us/download/details.aspx?id=8279
[compiler update for the Windows SDK 7.1]: http://www.microsoft.com/en-us/download/details.aspx?id=4422
