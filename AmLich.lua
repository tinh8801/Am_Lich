--[[
(c) 2006 Ho Ngoc Duc.
Astronomical algorithms
from the book "Astronomical Algorithms" by Jean Meeus, 1998

Port to LUA language by Huy Thang 2025
--]]

--[[
function YNT(d)
  return math.floor(d)
end
--]]

function jdFromDate(dd, mm, yy)
--[[
  jdFromDate(dd, mm, yy):
  Compute the (integral) Julian day number of day dd/mm/yyyy, i.e.,
  the number of days between 1/1/4713 BC (Julian calendar) and dd/mm/yyyy.
--]]

  a = math.floor((14 - mm)/12)
  y = yy + 4800 - a
  m = mm + 12*a - 3
  jd = dd + math.floor((153*m + 2) / 5) + 365*y + math.floor(y/4) -math.floor(y/100) + math.floor(y/400) - 32045
  if (jd < 2299161) then
        jd = dd + math.floor((153*m + 2)/5) + 365*y + math.floor(y/4) - 32083
  end
  return jd
end
  
function jdToDate(jd)
--jdToDate(jd): Convert a Julian day number to day/month/year. jd is an integer.
  if (jd > 2299160) then
    --After 5/10/1582, Gregorian calendar
        a = jd + 32044
        b = math.floor((4*a + 3) / 146097)
        c = a - math.floor((b*146097) / 4)
  else
        b = 0
        c = jd + 32082
  end
        d = math.floor((4*c + 3) / 1461)
        e = c - math.floor((1461*d) / 4)
        m = math.floor((5*e + 2) / 153)
        day = e - math.floor((153*m + 2) / 5) + 1
        month = m + 3 - 12*math.floor(m / 10)
        year = b*100 + d - 4800 + math.floor(m / 10)
  return {day, month, year}
end

function getNewMoonDay(k, timeZone)
--[[
NewMoon(k): Compute the time of the k-th new moon after the new moon of 1/1/1900 13:52 UCT (measured as the number of days since 1/1/4713 BC noon UCT, e.g., 2451545.125 is 1/1/2000 15:00 UTC. Returns a floating number, e.g., 2415079.9758617813 for k=2 or 2414961.935157746 for k=-2.
--]]
    --Time in Julian centuries from 1900 January 0.5
    T = k / 1236.85
    T2 = T * T
    T3 = T2 * T
    dr = math.pi / 180
    Jd1 = 2415020.75933 + 29.53058868*k + 0.0001178*T2 - 0.000000155*T3
    Jd1 = Jd1 + 0.00033*math.sin((166.56 + 132.87*T - 0.009173*T2)*dr)
    --Mean new moon
    M = 359.2242 + 29.10535608*k - 0.0000333*T2 - 0.00000347*T3
    --Sun's mean anomaly
    Mpr = 306.0253 + 385.81691806*k + 0.0107306*T2 + 0.00001236*T3
    --Moon's mean anomaly
    F = 21.2964 + 390.67050646*k - 0.0016528*T2 - 0.00000239*T3
    --Moon's argument of latitude
    C1 = (0.1734 - 0.000393*T)*math.sin(M*dr) + 0.0021*math.sin(2*dr*M)
    C1 = C1 - 0.4068*math.sin(Mpr*dr) + 0.0161*math.sin(dr*2*Mpr)
    C1 = C1 - 0.0004*math.sin(dr*3*Mpr)
    C1 = C1 + 0.0104*math.sin(dr*2*F) - 0.0051*math.sin(dr*(M + Mpr))
    C1 = C1 - 0.0074*math.sin(dr*(M - Mpr)) + 0.0004*math.sin(dr*(2*F + M))
    C1 = C1 - 0.0004*math.sin(dr*(2*F - M)) - 0.0006*math.sin(dr*(2*F + Mpr))
    C1 = C1 + 0.0010*math.sin(dr*(2*F - Mpr)) + 0.0005*math.sin(dr*(2*Mpr + M))
    if (T < -11) then
        deltat= 0.001 + 0.000839*T + 0.0002261*T2 - 0.00000845*T3 - 0.000000081*T*T3
    else
        deltat= -0.000278 + 0.000265*T + 0.000262*T2
    end
    JdNew = Jd1 + C1 - deltat
    return math.floor(JdNew + 0.5 + timeZone/24)
end
  

function getSunLongitude(jdn, timeZone)
--[[
SunLongitude(jdn): Compute the longitude of the sun at any time. Parameter: floating number jdn, the number of days since 1/1/4713 BC noon.
--]]
    
    T = (jdn - 2451545.5-timeZone/24) / 36525
    --Time in Julian centuries from 2000-01-01 12:00:00 GMT
    T2 = T * T
    dr = math.pi / 180
    M = 357.52910 + 35999.05030*T - 0.0001559*T2 - 0.00000048*T*T2
    --Mean anomaly, degree
    L0 = 280.46645 + 36000.76983*T + 0.0003032*T2
    --Mean longitude, degree
    DL = (1.914600 - 0.004817*T - 0.000014*T2) * math.sin(dr*M)
    DL = DL + (0.019993 - 0.000101*T) *math.sin(dr*2*M) + 0.000290*math.sin(dr*3*M)
    L = L0 + DL
    omega = 125.04 - 1934.136 * T
    L = L - 0.00569 - 0.00478 * math.sin(omega * dr)
    L = L * dr
    L = L - math.pi*2*(math.floor(L / (math.pi*2)))
    --Normalize to (0, 2*math.pi)
    return math.floor(L/math.pi*6)
end
    

function getLunarMonth11(yy, timeZone)
--[[
getLunarMonth11(yy, timeZone):  Find the day that starts the luner month 11of the given year for the given time .
--]]
  --off = jdFromDate(31, 12, yy) - 2415021.076998695
    off = jdFromDate(31, 12, yy) - 2415021
    k = math.floor(off / 29.530588853)
    nm = getNewMoonDay(k, timeZone)
    sunLong = getSunLongitude(nm, timeZone)
    --sun longitude at local midnight
    if (sunLong >= 9) then
        nm = getNewMoonDay(k - 1, timeZone)
    end
    return nm
end
  

function getLeapMonthOffset(a11, timeZone)
--[[
getLeapMonthOffset(a11, timeZone): Find the index of the leap month after the month starting on the day a11.
--]]
    
    k = math.floor((a11 - 2415021.076998695) / 29.530588853 + 0.5)
    last = 0
    i = 1 --start with month following lunar month 11
    arc = getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone)
    while (true)
    do
        last = arc
        i = i + 1
        arc = getSunLongitude(getNewMoonDay(k + i, timeZone), timeZone)
        if not(arc ~= last and i < 14) then
            break
        end
    end
    return i - 1
end
  

function S2L(dd, mm, yy, timeZone)
--[[
S2L(dd, mm, yy, timeZone = 7): Convert solar date dd/mm/yyyy to the corresponding lunar date.
--]]
    
    dayNumber = jdFromDate(dd, mm, yy)
    k = math.floor((dayNumber - 2415021.076998695) / 29.530588853)
    monthStart = getNewMoonDay(k + 1, timeZone)
    --alert(dayNumber + " -> " + monthStart)
    if (monthStart > dayNumber) then
        monthStart = getNewMoonDay(k, timeZone)
    end
        a11 = getLunarMonth11(yy, timeZone)
        b11 = a11
    if (a11 >= monthStart) then
        lunarYear = yy
        a11 = getLunarMonth11(yy - 1, timeZone)
    else
        lunarYear = yy + 1
        b11 = getLunarMonth11(yy + 1, timeZone)
    end
    
    lunarDay = dayNumber - monthStart + 1
    diff = math.floor((monthStart - a11) / 29)
    lunarLeap = 0
    lunarMonth = diff + 11
    
    if (b11 - a11 > 365) then
        leapMonthDiff = getLeapMonthOffset(a11, timeZone)
        if (diff >= leapMonthDiff) then
            lunarMonth = diff + 10
            if (diff == leapMonthDiff) then
              lunarLeap = 1
            end
        end
    end
          
    if (lunarMonth > 12) then
      lunarMonth = lunarMonth - 12
    end
    
    if (lunarMonth >= 11 and diff < 4) then
      lunarYear = lunarYear -1
    end
    return {lunarDay, lunarMonth, lunarYear, lunarLeap}
end

function L2S(lunarD, lunarM, lunarY, lunarLeap, tZ)
--[[
L2S(lunarD, lunarM, lunarY, lunarLeap, tZ = 7): Convert a lunar date to the corresponding solar date.
--]]
      
    if (lunarM < 11) then
        a11 = getLunarMonth11(lunarY - 1, tZ)
        b11 = getLunarMonth11(lunarY, tZ)
    else
        a11 = getLunarMonth11(lunarY, tZ)
        b11 = getLunarMonth11(lunarY + 1, tZ)
    end
    k = math.floor(0.5 + (a11 - 2415021.076998695) / 29.530588853)
    off = lunarM - 11
    
    if (off < 0) then
      off = off + 12
    end
    
    if (b11 - a11 > 365) then
        leapOff = getLeapMonthOffset(a11, tZ)
        leapM = leapOff - 2
        if (leapM < 0) then
          leapM = leapM + 12 
        end
        if (lunarLeap ~= 0 and lunarM ~= leapM) then
          return {0, 0, 0}
        elseif (lunarLeap ~= 0 or off >= leapOff) then
          off = off + 1
        end
    end
    monthStart = getNewMoonDay(k + off, tZ)
    return jdToDate(monthStart + lunarD - 1)
end

function CanChi(dd, mm, yy)
    Can={'Canh','Tân','Nhâm','Quý','Giáp','Ất','Bính','Đinh','Mậu','Kỷ'}
    Chi={'Thân','Dậu','Tuất','Hợi','Tí','Sửu','Dần','Mão','Thìn','Tỵ','Ngọ','Mùi'}
    if(dd < 0 or dd>31 or mm < 0 or mm>12 or yy < 0) then
      print('Ngày, tháng, năm không hợp lệ')
    else
        lDate = S2L(dd, mm, yy, 7)
        lYear = lDate[3]
        canIndex = lYear % 10
        chiIndex = lYear % 12
    end
    return Can[canIndex+1]..' '..Chi[chiIndex+1]
end

print('Ngày Dương Lịch: ', os.date('%d-%m-%Y'))
amlich=S2L(os.date('%d'), os.date('%m'), os.date('%Y'), 7)
isLeapMonth=''
yearName='('..CanChi(os.date('%d')+0, os.date('%m')+0, os.date('%Y')+0)..')'
if amlich[4]==1 then
    isLeapMonth='(nhuận)'
end
print('Ngày Âm Lịch: ', amlich[1], amlich[2], isLeapMonth, amlich[3], yearName)

duonglich=L2S(1, 6, 2025, 1, 7)
print('Ngày Âm Lịch 1-6 (nhuận)-2025')
print('là ngày: ', duonglich[1], duonglich[2], duonglich[3], ' dương lịch')