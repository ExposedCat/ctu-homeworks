# CTU FEL OI | PRP - Procedural Programming (for OI)
## Testing
1. Use `init` script to download & prepare homework data (N is number of the homework)
```bash
./init N
```
2. Go to the created homework directory
```bash
cd ./homework-N
```
3. Write program in `main.c`
4. Use `test` script to test & package homework data
```bash
../test
```
## TODO
- Patch `Makefile`s for `-Wall`, `-Werror` and `-Wextra` flags
- Automate valgrind checks
- Generate random tests