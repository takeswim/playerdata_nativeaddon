{
    "targets": [
        {
            "target_name": "addon",
            "sources": [ "playerdata.cc" ],
            "include_dirs" : [ "<!(node -e \"require('nan')\")"]
        }
    ]
}
