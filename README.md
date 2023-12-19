# lltree: Enhanced Tree Listing

The lltree command, a fusion of the tree and ll alias (ls -la)
command, is designed to provide an extended and comprehensive 
view of the file structure. This project aims to augment the tree 
command by incorporating additional details such as file 
permissions, user, and creation date. By doing so, it 
offers a swift and dependable solution for visualizing folder
contents while ensuring no crucial elements are overlooked.

## Usage:

- Use the make command to compile and install the lltree project:
```
make && make install
``` 

- The syntax is as follows:
```
lltree <path>
```
- `<path>` represents the target directory. It can be either an absolute or relative path.

- However, there is no need for a path if only the current directory needs to be disclosed.
```
lltree
```

- Maybe you have some problems with lltree or just want to remove it. No problem, just use:
```
make uninstall
```

## Example

```
$ lltree
```

![output example](https://github.com/DefNu1l/lltree/blob/main/assets/output_example.png)

# License
The lltree project is published under the [MIT](https://github.com/DefNu1l/lltree/blob/main/LICENSE) license.
