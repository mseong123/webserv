function smoothScroll() {
    var targetElement=document.getElementById(this.getAttribute("href").replace("#",""));
    var targetPosition=targetElement.offsetTop;
    window.scroll({
      top:targetPosition,
      behavior:"smooth",
    });
  }
  
  //attach eventlisteners and handlers on all navbar internal link element
  var navElements=document.querySelectorAll("a[href*='#']:not([href='#'])");
  
  navElements.forEach(function(element){
    element.addEventListener("click",function(e) {
    e.preventDefault();
    smoothScroll.call(this);   
  })
  })
  
  
  