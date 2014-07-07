var sten = require("../index.js");
var str = "hi this is some random <?stn var arr= ['s', 't', 'r', 'i', 'n', 'g'];  print(arr.join('-'))  stn?> to parse";

sten.createContext();
sten.parse(str, function(msg) {
    console.log(msg);
});
