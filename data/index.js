// Get the element with id="defaultOpen" and click on it
document.getElementById("defaultOpen").click();

function openTab(event, tabName) {
    var i, tabcontent, tablinks;
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
      tabcontent[i].style.display = "none";
    }
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
      tablinks[i].className = tablinks[i].className.replace(" active", "");
    }
    document.getElementById(tabName).style.display = "block";
    event.currentTarget.className += " active";
  }

function switchConfig(event, configName) {
    let i, checked, tabcontent;

    checked = event.currentTarget.checked;
    tabcontent = document.getElementsByClassName(configName);
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = checked ? "block" : "none";
    }
}