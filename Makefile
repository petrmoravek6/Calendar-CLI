LD=g++
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17
EXECUTABLE=calendar

all: compile

compile: $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)


$(EXECUTABLE): main.o calendarView.o calendarViewWeekly.o calendarViewDaily.o calendarViewMonthly.o event.o eventDayLayout.o jsonReader.o writer.o mainBox.o repeat.o repeatOnceTwoWeeks.o repeatDaily.o repeatOnceAWeek.o repeatEveryWeekday.o clock.o date.o timePeriod.o timeStamp.o id.o util.o
	$(LD) $^ -o $@

testUtil: testUtil.o clock.o date.o util.o timeStamp.o
	$(LD) $^ -o $@
	./testUtil

testEvent: testEvent.o calendarView.o calendarViewWeekly.o calendarViewDaily.o calendarViewMonthly.o event.o eventDayLayout.o jsonReader.o writer.o mainBox.o repeat.o repeatOnceTwoWeeks.o repeatDaily.o repeatOnceAWeek.o repeatEveryWeekday.o clock.o date.o timePeriod.o timeStamp.o id.o util.o
	$(LD) $^ -o $@
	./testEvent

testDate: testDate.o date.o
	$(LD) $^ -o $@
	./testDate

testJSONReader: testJSONReader.o calendarView.o calendarViewWeekly.o calendarViewDaily.o calendarViewMonthly.o event.o eventDayLayout.o jsonReader.o writer.o mainBox.o repeat.o repeatOnceTwoWeeks.o repeatDaily.o repeatOnceAWeek.o repeatEveryWeekday.o clock.o date.o timePeriod.o timeStamp.o id.o util.o
	$(LD) $^ -o $@
	./testJSONReader

testUtil.o: examples/tests/testUtil.cpp \
 examples/tests/../../src/time/date.h \
 examples/tests/../../src/time/clock.h
	${CXX} ${CXXFLAGS} -c $< -o $@

testEvent.o: examples/tests/testEvent.cpp \
 examples/tests/../../src/time/date.h \
 examples/tests/../../src/time/clock.h \
 examples/tests/../../src/event/event.h \
 examples/tests/../../src/event/../repeat/repeat.h \
 examples/tests/../../src/event/../repeat/../time/timePeriod.h \
 examples/tests/../../src/event/../repeat/../time/timeStamp.h \
 examples/tests/../../src/event/eventDayLayout.h
	${CXX} ${CXXFLAGS} -c $< -o $@

testDate.o: examples/tests/testDate.cpp \
 examples/tests/../../src/time/date.h
	${CXX} ${CXXFLAGS} -c $< -o $@

testJSONReader.o: examples/tests/testJSONReader.cpp \
 examples/tests/../../src/file/jsonReader.h \
 examples/tests/../../src/file/../event/event.h \
 examples/tests/../../src/file/../event/../repeat/repeat.h \
 examples/tests/../../src/file/../event/../repeat/../time/date.h \
 examples/tests/../../src/file/../event/../repeat/../time/timePeriod.h \
 examples/tests/../../src/file/../event/../repeat/../time/clock.h \
 examples/tests/../../src/file/../event/../repeat/../time/timeStamp.h \
 examples/tests/../../src/file/../event/eventDayLayout.h
	${CXX} ${CXXFLAGS} -c $< -o $@

main.o: src/main.cpp src/main_box/mainBox.h \
 src/main_box/../event/eventDayLayout.h \
 src/main_box/../event/../time/date.h \
 src/main_box/../event/../time/clock.h src/main_box/../event/event.h \
 src/main_box/../event/../repeat/repeat.h \
 src/main_box/../event/../repeat/../time/timePeriod.h \
 src/main_box/../event/../repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

calendarView.o: src/calendar_view/calendarView.cpp \
 src/calendar_view/calendarView.h src/calendar_view/../time/date.h \
 src/calendar_view/../event/event.h \
 src/calendar_view/../event/../repeat/repeat.h \
 src/calendar_view/../event/../repeat/../time/timePeriod.h \
 src/calendar_view/../event/../repeat/../time/clock.h \
 src/calendar_view/../event/../repeat/../time/timeStamp.h \
 src/calendar_view/../event/eventDayLayout.h \
 src/calendar_view/../util/util.h
	${CXX} ${CXXFLAGS} -c $< -o $@

calendarViewDaily.o: src/calendar_view/calendarViewDaily.cpp \
 src/calendar_view/calendarViewDaily.h src/calendar_view/calendarView.h \
 src/calendar_view/../time/date.h src/calendar_view/../event/event.h \
 src/calendar_view/../event/../repeat/repeat.h \
 src/calendar_view/../event/../repeat/../time/timePeriod.h \
 src/calendar_view/../event/../repeat/../time/clock.h \
 src/calendar_view/../event/../repeat/../time/timeStamp.h \
 src/calendar_view/../event/eventDayLayout.h
	${CXX} ${CXXFLAGS} -c $< -o $@

calendarViewMonthly.o: src/calendar_view/calendarViewMonthly.cpp \
 src/calendar_view/calendarViewMonthly.h src/calendar_view/calendarView.h \
 src/calendar_view/../time/date.h src/calendar_view/../event/event.h \
 src/calendar_view/../event/../repeat/repeat.h \
 src/calendar_view/../event/../repeat/../time/timePeriod.h \
 src/calendar_view/../event/../repeat/../time/clock.h \
 src/calendar_view/../event/../repeat/../time/timeStamp.h \
 src/calendar_view/../event/eventDayLayout.h
	${CXX} ${CXXFLAGS} -c $< -o $@

calendarViewWeekly.o: src/calendar_view/calendarViewWeekly.cpp \
 src/calendar_view/calendarViewWeekly.h src/calendar_view/calendarView.h \
 src/calendar_view/../time/date.h src/calendar_view/../event/event.h \
 src/calendar_view/../event/../repeat/repeat.h \
 src/calendar_view/../event/../repeat/../time/timePeriod.h \
 src/calendar_view/../event/../repeat/../time/clock.h \
 src/calendar_view/../event/../repeat/../time/timeStamp.h \
 src/calendar_view/../event/eventDayLayout.h
	${CXX} ${CXXFLAGS} -c $< -o $@

event.o: src/event/event.cpp src/event/event.h \
 src/event/../repeat/repeat.h src/event/../repeat/../time/date.h \
 src/event/../repeat/../time/timePeriod.h \
 src/event/../repeat/../time/clock.h \
 src/event/../repeat/../time/timeStamp.h src/event/eventDayLayout.h \
 src/event/../repeat/repeatEveryWeekday.h \
 src/event/../repeat/repeatOnceAWeek.h \
 src/event/../repeat/repeatOnceTwoWeeks.h src/event/../util/util.h \
 src/event/../main_box/mainBox.h
	${CXX} ${CXXFLAGS} -c $< -o $@

eventDayLayout.o: src/event/eventDayLayout.cpp src/event/eventDayLayout.h \
 src/event/../time/date.h src/event/../time/clock.h
	${CXX} ${CXXFLAGS} -c $< -o $@

jsonReader.o: src/file/jsonReader.cpp src/file/jsonReader.h \
 src/file/../event/event.h src/file/../event/../repeat/repeat.h \
 src/file/../event/../repeat/../time/date.h \
 src/file/../event/../repeat/../time/timePeriod.h \
 src/file/../event/../repeat/../time/clock.h \
 src/file/../event/../repeat/../time/timeStamp.h \
 src/file/fileException.h src/file/../repeat/repeatDaily.h \
 src/file/../repeat/repeatEveryWeekday.h \
 src/file/../repeat/repeatOnceAWeek.h \
 src/file/../repeat/repeatOnceTwoWeeks.h
	${CXX} ${CXXFLAGS} -c $< -o $@

writer.o: src/file/writer.cpp src/file/writer.h src/file/fileException.h
	${CXX} ${CXXFLAGS} -c $< -o $@
	
mainBox.o: src/main_box/mainBox.cpp src/main_box/mainBox.h \
 src/main_box/../event/eventDayLayout.h \
 src/main_box/../event/../time/date.h \
 src/main_box/../event/../time/clock.h src/main_box/../event/event.h \
 src/main_box/../event/../repeat/repeat.h \
 src/main_box/../event/../repeat/../time/timePeriod.h \
 src/main_box/../event/../repeat/../time/timeStamp.h \
 src/main_box/../calendar_view/calendarView.h \
 src/main_box/../calendar_view/calendarViewDaily.h \
 src/main_box/../calendar_view/calendarViewWeekly.h \
 src/main_box/../calendar_view/calendarViewMonthly.h \
 src/main_box/../util/util.h src/main_box/../repeat/repeatDaily.h \
 src/main_box/../repeat/repeatEveryWeekday.h \
 src/main_box/../repeat/repeatOnceAWeek.h \
 src/main_box/../repeat/repeatOnceTwoWeeks.h src/main_box/../util/id.h \
 src/main_box/../file/writer.h src/main_box/../file/fileException.h \
 src/main_box/../file/jsonReader.h
	${CXX} ${CXXFLAGS} -c $< -o $@
	
repeat.o: src/repeat/repeat.cpp src/repeat/repeat.h \
 src/repeat/../time/date.h src/repeat/../time/timePeriod.h \
 src/repeat/../time/clock.h src/repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

repeatDaily.o: src/repeat/repeatDaily.cpp src/repeat/repeatDaily.h \
 src/repeat/repeat.h src/repeat/../time/date.h \
 src/repeat/../time/timePeriod.h src/repeat/../time/clock.h \
 src/repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

repeatEveryWeekday.o: src/repeat/repeatEveryWeekday.cpp \
 src/repeat/repeatEveryWeekday.h src/repeat/repeat.h \
 src/repeat/../time/date.h src/repeat/../time/timePeriod.h \
 src/repeat/../time/clock.h src/repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

repeatOnceAWeek.o: src/repeat/repeatOnceAWeek.cpp \
 src/repeat/repeatOnceAWeek.h src/repeat/repeat.h \
 src/repeat/../time/date.h src/repeat/../time/timePeriod.h \
 src/repeat/../time/clock.h src/repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

repeatOnceTwoWeeks.o: src/repeat/repeatOnceTwoWeeks.cpp \
 src/repeat/repeatOnceTwoWeeks.h src/repeat/repeat.h \
 src/repeat/../time/date.h src/repeat/../time/timePeriod.h \
 src/repeat/../time/clock.h src/repeat/../time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

clock.o: src/time/clock.cpp src/time/clock.h
	${CXX} ${CXXFLAGS} -c $< -o $@

date.o: src/time/date.cpp src/time/date.h
	${CXX} ${CXXFLAGS} -c $< -o $@

timePeriod.o: src/time/timePeriod.cpp src/time/timePeriod.h \
 src/time/date.h src/time/clock.h src/time/timeStamp.h
	${CXX} ${CXXFLAGS} -c $< -o $@

timeStamp.o: src/time/timeStamp.cpp src/time/timeStamp.h src/time/date.h \
 src/time/clock.h
	${CXX} ${CXXFLAGS} -c $< -o $@

id.o: src/util/id.cpp src/util/id.h
	${CXX} ${CXXFLAGS} -c $< -o $@

util.o: src/util/util.cpp src/util/util.h src/util/../time/timeStamp.h \
 src/util/../time/date.h src/util/../time/clock.h
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	rm -f *.o *~ $(EXECUTABLE) testJSONReader testUtil testDate testEvent
