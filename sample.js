
var addon = require('./build/Release/prejs');
var fs = require('fs');





addon.createContext();

/*basic parser: *****************************************************************************************************************/
var str = "hi this is some random <?pjs var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  pjs?> to parse";

addon.parse(str, function(respone){
	console.log('__: parsed content:');
	console.log('js: '+respone);
});

addon.destroyContext();


/*basic parser with global settings: *****************************************************************************************************************/

var settings = {
		string: "global",
		data: [1, true, "data"]
}
addon.createContext(settings);

var str2 = "hi this is another sample using <?pjs this.number=4; print(string, ' ',  data[2])  pjs?> and 'this'; var a = '<?pjsprint(data[1])pjs?>'  and var b = '<?pjsprint(data[0], this.number)pjs?>';";

addon.parse(str2, function(respone){
	console.log('__: 2nd parsed content:');
	console.log('js: '+respone);
});


addon.loadFile('workdir/input.html', function(response) {
    console.log('  js callback');
    console.log(response);
    fs.writeFile('workdir/output.html', response);
});

// if the context is not destroyed you have access to variables defined in other files
addon.loadFile('workdir/input.css', function(response) {
	console.log('  js callback');
	console.log(response);
	fs.writeFile('workdir/output.css', response);

});





//console.log(a);
