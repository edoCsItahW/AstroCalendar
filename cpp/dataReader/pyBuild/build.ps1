Write-Output "加载CMake目标: 将build目录下的文件编译成可执行文件"
cmake -B build -S .

Write-Output "`n`n编译目标: 以Release模式编译build目录下的文件"
cmake --build build --config Release

$sourceDir = "./build/Release"
$targetDir = "../../python/reader"

# 验证源目录是否存在
if (-not (Test-Path $sourceDir)) {
    Write-Error "目录不存在: $sourceDir"
    exit 1
}

# 使用通配符匹配pyd文件
$pydFile = Get-ChildItem -Path $sourceDir -Filter "dataReader*.pyd" | Select-Object -First 1

if ($pydFile) {
    Move-Item -Path $pydFile.FullName -Destination "$targetDir/dataReader.pyd" -Force
    Write-Output "成功复制: $($pydFile.Name) -> $targetDir/dataReader.pyd"
}
else {
    Write-Error "在 $sourceDir 中未找到dataReader*.pyd文件"
    exit 1
}

Write-Output "进程结束: 推出码$LASTEXITCODE"
