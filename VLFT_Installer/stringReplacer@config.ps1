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
    Replace-TextInFile -FilePath $_.FullName -Pattern '-build/bin' -Replacement "/bin/release" 
}

Get-ChildItem . *.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/samples/virtualLearningFactory/resources' -Replacement $($pwd.Path.replace( '\', '/')  + '/samples/virtualLearningFactory/resources') 
}

Get-ChildItem . *.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/macros/sceneMaker/resources' -Replacement $($pwd.Path.replace( '\', '/')  + '/macros/sceneMaker/resources')
}

Get-ChildItem . *.json -rec | ForEach-Object { 
    Replace-TextInFile -FilePath $_.FullName -Pattern '/plugins/render/ogreRender/resources' -Replacement $($pwd.Path.replace( '\', '/') + '/plugins/render/ogreRender/resources')
}
