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

//	test_while(); // This does not work for some reason

	var string = "sample_string";
	var len = strlen(string);
	var firstchar = strchr(0);
	var lastchar = strchr(len - 1);
	
	
	if firstchar == lastchar {
		print("First and last characters match in: ", string);
	}

	var table = [];
	table["b"] = [];
	table["b"]["c"] = 3;
	table["b"]["d"] = table["b"]["c"]; // Add nested table read in factor() aswell. Copy code from statement()

	table["b"]["d"] = [];
	table["b"]["d"]["just_one_more_to_be_sure"] = 5;

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
