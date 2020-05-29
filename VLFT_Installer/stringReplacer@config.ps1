function Replace-TextInFile
{
    Param(
        [string]$FilePath,
        [string]$Pattern,
        [string]$Replacement
    )

    [System.IO.File]::WriteAllText(
        $FilePath,
        ([System.IO.File]::ReadAllText($FilePath) -replace $Pattern, $Replacement)
    )
}

Get-ChildItem . config.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern 'C:/ApertusVR09' -Replacement $pwd.Path.replace( '\', '/') 
}

Get-ChildItem . config.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '-build' -Replacement '' 
}
