sudo apt update
sudo apt install
sudo apt install g++
sudo apt install gdb
sudo apt install make
sudo apt install git
sudo apt-get install gdb
sudo apt-get install valgrind
sudo apt-get install gcovr
git clone https://gitexercises.fracz.com/git/exercises.git
cd exercises
git config user.name Murilo Maciel
git config user.email murilo-maciel@ufmg.br
./configure.sh
git start
git verify
git start next
git add A.txt
git commit "Adicionando apenas um arquivo"
git commit -m "Adicionando apenas um arquivo"
git verify
git satart next
git start next
git reset B.txt
git commit -m "Commit only A.txt"
git verify
git start next
.gitignore
touch .gitignore
*.exe
*.o
*.jar
libraries/
*.exe
echo "*.exe" > .gitignore
echo "*.o" >> .gitignore
echo "*.jar" >> .gitignore
echo "libraries/" >> .gitignore
cat .gitignore
git add .gitignore
git commit -m "Ignoring files"
git verify
git start next
git merge escaped
git verify
git start next
git merge another-piece-of-work
git status
nano equation.txt
git add equation.txt
git commit -m "conflito resolvido"
git verify
nano equation.txt
git add equation.txt
git commit -m "conflito resolvido"
git verify
git reset --soft HEAD~1
git merge --continue
git commit --no-edit
git verify
nano equation.txt
git add equation.txt
git commit --amend --no-edit
git verify
git start next
git stash
nano bug.txt
git add bug.txt
git commit -m "Bug fix"
git stash pop
echo "Finally, finished it!" >> bug.txt
cat bug.txt
git add .
git commit -m "Trabalho finalizado após correção de bug"
git verify
git clone https://gitexercises.fracz.com/git/exercises.git
cd exercises
git config user.name Murilo Maciel
git config user.email murilo-maciel@ufmg.br
./configure.sh
git start
git start remove-ignored
git rm --cached ignored.txt
git add .
git commit -m "Removendo arquivo ignorado do rastreamento"
git verify
git clone https://github.com/WillianJunior/PDS2-2026-PF-grupo12.git
cd PDS2-2026-PF-grupo12
nano 2025014613.txt
git status
git add -A
git commit -m "Adicionando minha matrícula conforme solicitado para o PF"
git config --global user.email murilomaciel1577@gmail.com
git config --global user.name "Murilo Maciel
"
git commit -m "Adicionando minha matrícula conforme solicitado"
git push
git pus
git push
git clone https://github.com/WillianJunior/PDS2-2026-PF-grupo12.git
cd PDS2-2026-PF-grupo12
nano 2025014613.txt
git status
git push
