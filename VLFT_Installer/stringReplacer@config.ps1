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

Get-ChildItem . apeCore.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/samples/virtualLearningFactory/models/' -Replacement $($pwd.Path.replace( '\', '/')  + '/samples/virtualLearningFactory/resources/') 
}

Get-ChildItem . apeCore.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/macros/sceneMaker/resources' -Replacement $($pwd.Path.replace( '\', '/')  + '/macros/sceneMaker/resources/')
}

Get-ChildItem . apeCore.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/plugins/render/ogreRender/resources/' -Replacement $($pwd.Path.replace( '\', '/') + '/plugins/render/ogreRender/resources/')
}
