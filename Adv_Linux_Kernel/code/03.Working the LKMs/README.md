# Linux Kernel Fundamentals: Chapter 3, Working with Loadable Kernel Modules

## Basics

### List Modules

```
lsmod
```

### Module Info
```
modinfo <module>
```

### Add Modules
```
insmod <module>.ko
```
**Does not add dependent Modules**

### Remove Modules
```
rmmod <module>
```
**Does not remove the dependent Modules**

### `modprobe` Command
#### Add Modules
```
modprobe -a <modules>
```
**Loads All dependent modules too**

#### Remove Modules
```
modprobe -r <modules>
```
**Remove all dependent modules if not in use**
