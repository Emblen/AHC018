var array = [];
for(var i=0; i<10; i++){
    array[i] = [];
    for(var j=0; j<10; j++){
        array[i][j] = i*j;
        console.log(array[i][j]);
    }
}