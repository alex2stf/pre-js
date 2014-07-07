var sten = require('../build/Release/sten');
var fs = require('fs');


sten.createContext();

/*basic parser: *****************************************************************************************************************/
var str = "hi this is some random <?stn var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  stn?> to parse";

sten.parse(str, function(respone){
	console.log('__: parsed content:');
	console.log('js: '+respone);
});

sten.destroyContext();


/*basic parser with global settings: *****************************************************************************************************************/

var settings = {
		string: "arguments",
		data: [1, true, "data"]
}
sten.createContext(settings);

var str2 = "hi this is another sample using <?stn this.number=4; print(string, ' ',  data[2])  stn?> and 'this': var a = '<?stnprint(data[1])stn?>'  and var b = '<?stnprint(data[0], this.number)stn?>';";

sten.parse(str2, function(respone){
	console.log('js: 2nd parsed content:');
	console.log('js: '+respone);
});


sten.loadFile('test/workdir/input.html', function(response) {
    console.log('js:'+response);
    fs.writeFile('test/workdir/output.html', response);
});

/* if the context is not destroyed you have access to variables defined in previous file  */
sten.loadFile('test/workdir/input.css', function(response) {
	console.log('js:'+response);
    fs.writeFile('test/workdir/output.css', response);

});


var x = sten.cin("and this a cin method: just type something >> ");
console.log('you typed "'+x+'" THANKS!');

