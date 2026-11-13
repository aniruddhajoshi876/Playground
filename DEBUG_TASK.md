# Debug task: RTT Viewer + SystemView

## The system
Three FreeRTOS tasks share one bus (see `Core/Src/main.c`):

| Task      | Priority | Job                                                    |
|-----------|----------|--------------------------------------------------------|
| Control   | 3 (high) | 20 Hz loop, needs the bus each cycle, 25 ms deadline   |
| Telemetry | 2        | Builds and sends a frame once a second                 |
| Sensor    | 1 (low)  | Reads a sensor over the bus every ~50 ms               |

## Symptom
Control is supposed to meet its deadline every cycle. Every so often it doesn't.
Nothing crashes and nothing asserts.

## Your job
1. Flash the firmware and open RTT Viewer. Describe what you see: how often does it fail, and is there a pattern?
2. Form a hypothesis from the RTT output alone. Write it down before you look further.
3. Add SEGGER SystemView to the project (target sources + FreeRTOS hooks) and record a trace that catches a failure.
4. Use the timeline to confirm or kill your hypothesis. Which task was running while Control was stuck, and why?
5. Fix it, re-record, and show the failures are gone.

## Rules
- Fix it in the code. Do not just change timings or priorities to hide it.
- No peeking at `git log` for hints. This branch has no spoilers there.

## Notes
- RTT output comes through `printf` (`PRINTF_TO_RTT` in `Core/Src/syscalls.c`).
- The project also contains Percepio TraceRecorder, which uses the same FreeRTOS trace hooks SystemView needs. Working out how the two coexist is part of the task.
- J-Scope: `ctlWaitMs`, `ctlCycleMs` and `ctlMissed` in `main.c` are plain globals you can plot. Point J-Scope at `Debug/Led.elf`, pick the device `STM32G491VE`, and add them. Use the J-Link only, since ST-LINK can't sample memory like this. Plot them next to the RTT log and the SystemView timeline and see which tells the story best.
