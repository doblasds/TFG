!function(a){"use strict";var m=!1;function e(){if(m)return!1;try{var e=void 0!==(o=a.performance||a.webkitPerformance||a.msPerformance||a.mozPerformance)&&o.timing||!1;if(e&&_uptime_rum&&_uptime_rum.hasOwnProperty("uuid")){var t={type:"rumdata",data:{rumdata:{timing:e,user:{href:a.location.href,userAgent:a.navigator.userAgent}},uuid:_uptime_rum.uuid}},r=(u="POST",n=_uptime_rum.url,void 0!==(i=new XMLHttpRequest).withCredentials?i.open(u,n,!0):"undefined"!=typeof XDomainRequest?(i=new XDomainRequest).open(u,n):i=null,i);if(!r)return!1;r.onload=function(){},r.send(JSON.stringify(t))}else _uptime_rum&&!_uptime_rum.hasOwnProperty("uuid")&&console.log("You are missing _uptime_rum.uuid property which needs to be global.");return m=!0}catch(e){return!1}var u,n,i,o}_uptime_rum&&(_uptime_rum.sendReport=e),_uptime_rum&&_uptime_rum.hasOwnProperty("noAutoReport")||(a.addEventListener?a.addEventListener("load",e,!1):a.attachEvent?a.attachEvent("onload",e):a.onload=e)}(window);