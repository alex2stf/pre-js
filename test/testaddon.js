//git add . --all
//git commit -m "adding npm details"
//git push -u origin master


var preJs = require('../build/Release/prejs');
var fs = require('fs');


preJs.createContext();

/*basic parser: *****************************************************************************************************************/
var str = "hi this is some random <?pjs var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  pjs?> to parse";

preJs.parse(str, function(respone){
	console.log('__: parsed content:');
	console.log('js: '+respone);
});

preJs.destroyContext();


/*basic parser with global settings: *****************************************************************************************************************/

var settings = {
		string: "arguments",
		data: [1, true, "data"]
}
preJs.createContext(settings);

var str2 = "hi this is another sample using <?pjs this.number=4; print(string, ' ',  data[2])  pjs?> and 'this': var a = '<?pjsprint(data[1])pjs?>'  and var b = '<?pjsprint(data[0], this.number)pjs?>';";

preJs.parse(str2, function(respone){
	console.log('__: 2nd parsed content:');
	console.log('js: '+respone);
});


preJs.loadFile('test/workdir/input.html', function(response) {
    console.log(response);
    fs.writeFile('test/workdir/output.html', response);
});

// if the context is not destroyed you have access to variables defined in other files
preJs.loadFile('test/workdir/input.css', function(response) {
	console.log(response);
    fs.writeFile('test/workdir/output.css', response);

});


var x = preJs.cin("and this a cin method: just type something >> ");
console.log('you typed "'+x+'" THANKS!');





//console.log(a);
