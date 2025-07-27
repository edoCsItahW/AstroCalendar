Write-Output "加载CMake目标: 将build目录下的文件编译成可执行文件"
cmake -B build -S .

Write-Output "`n`n编译目标: 以Release模式编译build目录下的文件"
cmake --build build --config Release

Write-Output "`n`n复制目标: 将编译后的文件复制到test目录下"
Move-Item -Path "./build/Release/dataReader.cp312-win_amd64.pyd" -Destination "../../python/reader/dataReader.pyd" -Force

Write-Output "进程结束: 推出码$LASTEXITCODE"
