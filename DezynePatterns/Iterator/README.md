# Iterator pattern

Dezyne supports iteration by tail recursion. To deal with this in a nice and abstract way an iterator may be used.
The implementation of the iterator requires a few lines of hand written code.

The interface of the iterator would look as follows

```cpp
extern string $std::string$;
interface IIter
{
	subint number {0..9};
	in void myId(out string Id);		// provides Id of this Object
	in number numObjects();
	in bool nextObject(out string Id);	// provides Id of an Object, could also be pointer
	behaviour
	{
		on myId: {}
		number numberOfObjects = 4; // must be <= 9
		number object = 0;
		on numObjects: {reply(numberOfObjects);}
		on nextObject: {
			if(object < numberOfObjects){
				object = object + 1;
				reply(true);
			} else {
				reply(false);
			}
		}
	}
}
```

The iterator assumes there is a mapping from a unique indentifier (string) to the object to be retrieved.
In Dezyne the string identifier can be defined as an extern type.

The implementation of the iterator is in hand-written code. 
Objects could be retained in a linked list where the identifier is a pointer (or handle) to the object in the list.
Alternatively it could be an array with the index number as identifier.

An example of a tail recursive function in Dezyne:
```cpp
void initObjects() {
	string id;
	bool more = iter.nextObject(id);
	if (more) {
		bool skip_init = object.started(id);
		if (!skip_init) object.init(id);
   		initObjects(); // tail recursion      	
	}
}
```

Another example of a recursive function with a non-void return value:

```cpp
bool allObjectsStarted() {
	string id;
	bool result = object.started(id);
	if (result) {
		bool more = iter.nextObject(id);
    	if (more) {
    		result = allObjectsStarted(); // tail recursion      	
    	}
	}
	return result;
}
```

See the full .dzn file for an example how this is embedded in a Dezyne component.
