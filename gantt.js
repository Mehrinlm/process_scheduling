





  var size = 20;
  var currentY = 0;
  
  $.get("rts_output.txt", function (data){

    var the_data = data.split(/\n/);
    var process;
    var yStart = (the_data.length-5)*size;
    var paper = Raphael(0, 0, 100000, yStart);
    for (var i = 4; i < the_data.length-1; i++){
      process = the_data[i].split(/\s/);
      addBar(process, currentY, paper);
    }
    
    for (var i = 0; i < process[3]; i++){
      paper.text((i * size + 5), currentY+5, i + "");
    }

    currentY += yStart;

  }, "text");
 

  



function addBar(process, yStart, paper){
  var startX = process[2] * size;
  var startY = process[0]*size + size;
  
  var endX = process[3] * size;
  var endY = process[0]*size + 2*size;
  
  
  var rect = paper.rect(startX + 5, yStart + startY + 5, endX-startX, endY-startY);
  var text = paper.text(startX - size/2 + 5, yStart + startY - size/2 + 5, "P_ID: " + process[1]);
  if (process[4] == "*"){
    rect.attr("fill", "#f00");
    text.attr("fill", "#f00");
  } else {
    rect.attr("fill", "#00f");
  }
}