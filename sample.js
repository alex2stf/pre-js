
var addon = require('./build/Release/prejs');
var fs = require('fs');




var settings = {
		string: "global",
		data: [1, true, "data"]
}
addon.createContext(settings);


var str = "hi this is some random <?pjs var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  pjs?> to parse";

addon.parse(str, function(respone){
	console.log('js: parsed content:');
	console.log('js: '+respone);
});



var str2 = "hi this is another sample using <?pjs print(_global.string, _global.data[2])  pjs?>; var a = '<?pjsprint(_global.data(0))pjs?>'  and var b = '<?pjsprint(_global.data[1])pjs?>';";

addon.parse(str2, function(respone){
	console.log('js: parsed content:');
	console.log('js: '+respone);
});

//
//addon.loadFile('workdir/input.html', function(response) {
//    console.log('  js callback');
//    console.log(response);
//
//    try {
//        fs.writeFile('workdir/output.html', response);
//    } catch (e) {
////        console.log(e);
//    }
//});
//
//addon.loadFile('workdir/input.css', function(response) {
//	console.log('  js callback');
//	console.log(response);
//	
//	try {
//        fs.writeFile('workdir/output.css', response);
//	} catch (e) {
////	    console.log(e);
//	}
//});





//console.log(a);
