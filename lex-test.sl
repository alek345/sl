// Single line comment consuming everything until the end of the line func {}]{{[

/*
Multi line comment
*/

var line = 1;
var someunassignedglobal;

func test_while() {
	var i = 10;
	while i >= 0 {
		print(i, " out of 10");
		i = i - 1;
	}
}

func main(args) {
	if args["length"] != 0 {
		print("This program takes no arguments!");
		return;
	}

	if firstchar == lastchar {
		print("First and last characters match in: ", string);
	}

	var string = "sample_string";
	var len = strlen(string);
	var firstchar = strchr(0);
	var lastchar = strchr(len - 1);

	a["b"]["c"] = 3;
	/*
		if we introduce a "dot" syntax this would be the same as:

			a.b.c = 3;
	*/

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
