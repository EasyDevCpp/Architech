<h1 align="center">
	<br/>
	<img src="https://raw.githubusercontent.com/EasyDevCpp/Architech/master/img/Architech_v2_alpha.png" alt="Architech" width="80%"/>
	<br/>
	Architech
	<br/>
</h1>

<h4 align="center">
	Simple but yet effective programming language with C & C++ support.
	<a href="https://travis-ci.org/EasyDevCpp/Architech">
		<img src="https://travis-ci.org/EasyDevCpp/Architech.svg?branch=master" alt="Architech build"/>
	</a>
</h4>

> Simple but yet effective programming language with C & C++ support.
>
> *This is a fun project and nothing to be taken too seriously.* 

**CURRENTLY ONLY SUPPORTS LINUX AND SHOULDN'T BE USED BY ANYONE!**

Development might stop for the next few weeks, but after that will get back to work!

### Table of Contents 

> To make things easier for you <3

- [Requirements](#requirements)
- [Installation](#installation)
- [Features](#features)
- [Usage Example](#usage-example)
- [Motivation & Aims](#motivation-and-aims)
- [Release History](#release-history)
- [Meta](#meta)
- [Contributing](#contributing)

---

### Requirements

Architech requires the following elements:

- [GCC >= 4.9](https://gcc.gnu.org/)
- The linux distribution you prefer

> It might be possible to get it to run under WIndows, but currently I don't own a Windows plattform so I can't test it nor imply that I support it! The same applies to Mac OS and any other OS out there...
>
> Tested with: **Manjaro & GCC**
>
> Should work with: **Any linux distribution**

### Installation

1. Download the newest archive from GitHub and extract it

2. Open up your terminal and cd into the folder

3. Run "sudo bash ./install.sh"

4. Enjoy!


### Features

**Architech is currently still under heavy development and shouldn't be used by anyone for an entire project!**

Current features:

> Syntax-Check for each item included. Maybe there are still some missing checks cause the Syntax-Checker is still in an early state!

* Function definitions
* Variable definitions
* Default types
* Import 
* Returning values
* Pointers & references
* A decent io module(just console in- & output)(No overloads!!! They will come later!)
* architech_manager to simplify project creation
* Tips for faster hotfixes
* Default values

Future features:

- [ ] structs & classes
- [ ] inheritance
- [ ] SmartPointers
- [ ] array & dym_array
- [ ] import modules
- [ ] string libary
- [ ] compiler macros(@hello_world, @include_once, @set_get_methods)
- ...

**If you are interested in further directions this language will take than just read the comment block at the top of the basics.arch file!**

### Usage Example

1. Open up your favourite terminal

2. Run the following command:

   ```bash
   >> architech_manager new [Project Name]
   ```

3. Now open up the created file in your favourite editor:

   ```
   The created file will be located at:
   ~/Projects/[Project Name]/src/[Project Name].arch
   
   The include files will be located at:
   ~/Projects/[Project Name]/inc/
   ```

4. It should look like something like this:

   ```assembly
   import ./inc/basics.arch
   
   func main()
   	# Happy coding :D
   	ret
   end
   ```

5. Now let's start to code! But before that we have to clear out some things:

   ```assembly
   # Is a one line comment
   
   import #Imports a certain file. In our case it's the entire Architech standard libary
   
   func main() #Declares the main() function.
   	#the main function is always the first thing which gets executed
   	...
   end
   
   ret #Return a certain value. In this case it's just the default value of the type int
   ```

6. Ok now let's really start to code... Enter the following lines so the file looks like this:

   ```assembly
   #cs
   	This program is a demonstartion of the
   	basics.arch import-file!
   #ce
   
   import ./inc/basics.arch
   
   func main()
   	var $name = ""
   	println("Hello!\nWhat's your name?")
   	print("My name: ")
   	$name = read()
   	println("Hello, " + $name + "!")
   	ret
   end
   ```

7. Open up a terminal and enter the following command:

   ```bash
   >> architech_manager build
   ```

8. Run our program!

   ```
   >> architech_manager run
   
   ...or...
   
   >> ./[Project Name]
   ```

9. That's it! For further information please visit our [documentation](https://github.com/EasyDevCpp/Architech/blob/master/DOC.md).

### Motivation & Aims 

Look at the following C++ code:

```cpp
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;
    return 0;
}
```

Do you think this looks simple and satisfying enough? - *Yes to a certain degree.*

**BUT** and it's a big but... it can be done better!

Now look at the following code:

```assembly
import ./basics.arch

func main()
	print("Hello World!")
	ret
end
```

Do you think this looks better? - *I think so.*

First thing that I like is the fact that there are no brackets('{}') at all. The second thing I really like is the minimalism! No semicolons at the end of nearly each line, no need of some strange operators and no need to define the type of main(). I mean come on... It has to be int! There is even a error message for a wrong implementation of main()!

Also what really catches my eye is the simple use of the english language instead of using complex not so easy structures... Also the keywords always try to imply what they are doing, e.g. func. 

Now you might think I am a C++ hater but I am 100%ly not! Why else would I programm the entire compiler in C++? Why else would I convert the Architech code to C++? - *I wouldn't do anything of these things if I were against C++!*

But as many others, I believe that C++ is way too complex and besides some things are getting easier other things are getting more complex exponentially... So why not build a beginner friendly, fully fletched language on top of that? - *This is the moment when Architech was born.*

First thought I had was that I didn't want to make a simple copy of C++, but on the other hand I wanted to keep all the things I like the most about using C++. So figuring out the syntax was a hard time, because my aims where way too high but I think the outcome is *good enough* to go on with it.

What should Architech be...

* ...beginner friendly
* ...appealing for the eye
* ...not C++, but don't miss the core features
* ...open to complexity if the user wants it
* ...simple and minimalistic

### Release History

* **v0.0.1 - 31/08/2018**
    * *Added this document*
    * *Added documentation*
    * *Added core functionality*

### Meta

Robin Krause – [@EasyDevCpp](https://twitter.com/EasyDevCpp) – easy-dev[at]web.de

Distributed under the GNU General public license v3. See ``LICENSE`` for more information.

[https://github.com/EasyDevCpp/Architech](https://github.com/EasyDevCpp/Architech)

### Contributing

1. Fork it (<https://github.com/EasyDevCpp/Architech/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

### Thanks for reading!
