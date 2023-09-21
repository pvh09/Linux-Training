read -p "Enter filename " FileName

if [ -e $FileName ]
then
  echo "$FileName exists"
  if [ -r $FileName ]
  then
    echo "$FileName has read access"
  else 
    echo "$FileName does not have read access!"
  fi
  if [ -w $FileName ]
  then
    echo "$FileName has write access"
  else 
    echo "$FileName does not have write access!"
  fi
  if [ -x $FileName ]
  then
    echo "$FileName has execute access"
  else 
    echo "$FileName does not have execute access!"
  fi

  if [ -s $FileName ]
  then
    echo "$FileName size is non-zero"
  else 
    echo "$FileName is empty"
  fi

else 
  echo "$FileName not found!"
fi

