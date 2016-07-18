// Single line comment consuming everything until the end of the line func {}]{{[

/*
Multi line comment
*/

var line = 1;
var someunassignedglobal;

func main(args) {
	if args["length"] != 0 {
		print("This program takes no arguments!");
		return;
    }

	print("Hello, world");

	var a = 123;
	var b = 321;
	var c = (a + b) / 2;

	var person = [];
	person["age"] = 13;
	person["name"] = "Bob";

	if c == a {
		print("nope");
	} else {
		print("yay!");
	}
}