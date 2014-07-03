
var addon = require('./build/Release/prejs');
var fs = require('fs');



console.log(addon);

addon.createContext();

addon.loadFile('workdir/input.html', function(response) {
    console.log('  js callback');
    console.log(response);

    try {
        fs.writeFile('workdir/output.html', response);
    } catch (e) {
//        console.log(e);
    }
});

addon.loadFile('workdir/input.css', function(response) {
	console.log('  js callback');
	console.log(response);
	
	try {
        fs.writeFile('workdir/output.css', response);
	} catch (e) {
//	    console.log(e);
	}
});





//console.log(a);
