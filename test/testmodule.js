var sten = require("../index.js");
var str = "hi this is some random <?pjs var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  pjs?> to parse";

sten.createContext();
sten.parse(str, function(msg) {
    console.log(msg);
});
