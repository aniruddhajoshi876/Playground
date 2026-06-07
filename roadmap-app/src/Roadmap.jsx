import { useEffect, useState } from "react";

const STORAGE_KEY = "stm32-roadmap-state";

const phases = [
  {
    id: 1,
    title: "Phase 1",
    subtitle: "Core Control",
    color: "#22c55e",
    weeks: "Weeks 1-3",
    projects: [
      {
        name: "1.1 - GPIO Blink",
        desc: "Turn the LED on and off with HAL_GPIO_WritePin and HAL_GPIO_TogglePin. Learn push-pull output, pin config in CubeMX, and the HAL_Delay loop.",
        key: "MODER, ODR, BSRR registers. Push-pull vs open-drain.",
      },
      {
        name: "1.2 - Delay-Free Blink",
        desc: "Replace HAL_Delay with elapsed-time logic using HAL_GetTick(). The LED blinks but the main loop never blocks.",
        key: "Non-blocking pattern: if (now - last >= interval).",
      },
      {
        name: "1.3 - Timer Interrupt Blink",
        desc: "Configure a hardware timer (TIM2/TIM6) to fire an update interrupt at a set rate. Toggle the LED inside the ISR callback.",
        key: "Prescaler, ARR, NVIC, HAL_TIM_PeriodElapsedCallback.",
      },
      {
        name: "1.4 - Morse Code Blinker",
        desc: "Encode a message as dots and dashes. Use a state machine plus non-blocking timing to blink SOS or any string.",
        key: "Finite state machines, timing arrays, enum states.",
      },
      {
        name: "1.5 - Multi-Pattern State Machine",
        desc: "Cycle through modes: OFF -> SLOW -> FAST -> BURST -> repeat. A timer tick drives transitions.",
        key: "State enum, switch/case, transition logic.",
      },
    ],
  },
  {
    id: 2,
    title: "Phase 2",
    subtitle: "Deeper STM32 Knowledge",
    color: "#3b82f6",
    weeks: "Weeks 4-6",
    projects: [
      {
        name: "2.1 - Register-Level GPIO",
        desc: "Rewrite your blink entirely with direct register access. Enable the clock via RCC, set MODER, write to BSRR. No HAL calls.",
        key: "RCC->AHBxENR, GPIOx->MODER, GPIOx->BSRR.",
      },
      {
        name: "2.2 - Timer Registers",
        desc: "Configure TIM2 by writing PSC, ARR, and CR1 directly. Enable the update interrupt via DIER and NVIC.",
        key: "TIMx->PSC, ARR, CNT, CR1, DIER, SR.",
      },
      {
        name: "2.3 - PWM Breathing LED",
        desc: "Set up a timer in PWM mode. Sweep the CCR value smoothly to fade the LED in and out. Duty cycle = CCR/ARR.",
        key: "OC mode, CCRx, PWM edge-aligned mode.",
      },
      {
        name: "2.4 - Clock Tree Exploration",
        desc: "Switch between HSI and HSE. Change the PLL multiplier. Observe that your blink timing shifts.",
        key: "HSI, HSE, PLL, AHB/APB prescalers, SysTick.",
      },
      {
        name: "2.5 - SysTick Scheduler",
        desc: "Build a cooperative scheduler on SysTick. Register 3 tasks at different rates. The LED shows activity.",
        key: "SysTick_Handler, task table, flags, period counters.",
      },
      {
        name: "2.6 - Watchdog Reset Demo",
        desc: "Enable IWDG. Refresh it in the main loop. Then intentionally stop refreshing - observe the MCU reset.",
        key: "IWDG prescaler, reload, KR register, reset source flags.",
      },
    ],
  },
  {
    id: 3,
    title: "Phase 3",
    subtitle: "Architecture & Abstraction",
    color: "#a855f7",
    weeks: "Weeks 7-9",
    projects: [
      {
        name: "3.1 - LED Driver Module",
        desc: "Create led.h / led.c with init, on, off, toggle, set_pattern. Decouple the LED from main.",
        key: "Encapsulation, API design, header guards.",
      },
      {
        name: "3.2 - Timer Abstraction Layer",
        desc: "Write a timer_manager that lets you request periodic callbacks without touching registers directly.",
        key: "Function pointers, callback registration, abstraction.",
      },
      {
        name: "3.3 - HAL vs LL vs Register Benchmark",
        desc: "Implement the same blink three ways. Measure toggle speed. See the overhead difference.",
        key: "HAL, LL inline functions, direct BSRR write. Cycle counting.",
      },
      {
        name: "3.4 - Board Manager + State Machine",
        desc: "Build a board_manager that owns system state (BOOT -> IDLE -> ACTIVE -> FAULT). The LED driver reads state and shows the correct pattern.",
        key: "Architecture layers: app -> manager -> driver -> hardware.",
      },
      {
        name: "3.5 - Error-Code LED Framework",
        desc: "Design a system where fault codes are blinked as patterns. A fault_handler sets codes, the LED driver displays them.",
        key: "Error tables, blink encoding, decoupled reporting.",
      },
    ],
  },
  {
    id: 4,
    title: "Phase 4",
    subtitle: "Peripheral Exploration",
    color: "#f59e0b",
    weeks: "Weeks 10-13",
    projects: [
      {
        name: "4.1 - UART printf Redirect",
        desc: "Redirect printf over the ST-LINK virtual COM port. Print status messages while the LED blinks.",
        key: "USART BRR, retarget _write / fputc, async serial framing.",
      },
      {
        name: "4.2 - ADC Internal Channels",
        desc: "Read the internal temperature sensor and Vrefint via ADC. Print values over UART.",
        key: "ADC resolution, sampling time, calibration, rank/sequence.",
      },
      {
        name: "4.3 - DMA Memory Transfer",
        desc: "Set up DMA to copy a buffer in memory. Then use DMA to feed UART TX. Normal vs circular mode.",
        key: "DMA channel, src/dst, CNDTR, CMAR, CPAR, TC interrupt.",
      },
      {
        name: "4.4 - Low-Power Sleep / Wake",
        desc: "Enter Sleep mode after blinking. Wake on a timer interrupt. Then try Stop mode with RTC wakeup.",
        key: "WFI, SLEEPDEEP, Stop mode, RTC wakeup, clock reinit.",
      },
      {
        name: "4.5 - FreeRTOS LED Controller",
        desc: "Two tasks: one decides the blink pattern (producer), one drives the LED (consumer). Communicate via a queue.",
        key: "xTaskCreate, xQueueSend/Receive, vTaskDelay, preemption.",
      },
      {
        name: "4.6 - Boot / Startup / Linker Deep Dive",
        desc: "Read your startup_stm32*.s and linker script. Find the vector table, Reset_Handler, stack init. Trace what runs before main().",
        key: "Vector table, .isr_vector, _estack, SystemInit, __libc_init.",
      },
    ],
  },
  {
    id: 5,
    title: "Phase 5",
    subtitle: "Conceptual Mastery",
    color: "#ef4444",
    weeks: "Weeks 14-16",
    projects: [
      {
        name: "5.1 - SPI Driver Skeleton",
        desc: "Configure SPI master. Write a full driver API even without external hardware. Inspect SPI registers and flags.",
        key: "SCK, MOSI, MISO, NSS, CPOL, CPHA, SPI_SR flags.",
      },
      {
        name: "5.2 - I2C Driver Skeleton",
        desc: "Configure I2C master. Write address/read/write functions. Study START, STOP, ACK/NACK sequences.",
        key: "SDA, SCL, 7-bit addressing, open-drain, pull-ups.",
      },
      {
        name: "5.3 - CAN / FDCAN Study",
        desc: "Configure CAN peripheral. Attempt a transmit - it will fail without a second node, but you will see the TX error counter increment.",
        key: "CAN ID, arbitration, bit timing, filters, FIFO, error frames.",
      },
      {
        name: "5.4 - Fault Handler Deep Dive",
        desc: "Intentionally trigger HardFault. Implement a fault handler that blinks an error code and prints the stacked PC/LR.",
        key: "HardFault_Handler, stacked registers, CFSR, BFAR, MMFAR.",
      },
      {
        name: "5.5 - Capstone: Full Embedded Framework",
        desc: "Combine everything: board manager, LED driver, timer abstraction, scheduler, UART logging, watchdog, fault handler, state machine - one clean project.",
        key: "Layered architecture. The one LED shows system state at every level.",
      },
    ],
  },
];

function loadState() {
  try {
    const raw = window.localStorage.getItem(STORAGE_KEY);
    if (!raw) {
      return { done: {}, notes: {} };
    }

    const parsed = JSON.parse(raw);
    return {
      done: parsed.done || {},
      notes: parsed.notes || {},
    };
  } catch {
    return { done: {}, notes: {} };
  }
}

function saveState(done, notes) {
  try {
    window.localStorage.setItem(STORAGE_KEY, JSON.stringify({ done, notes }));
  } catch {
    // Ignore storage failures so the UI still works.
  }
}

export default function Roadmap() {
  const [openPhase, setOpenPhase] = useState(1);
  const [done, setDone] = useState({});
  const [notes, setNotes] = useState({});
  const [editingNote, setEditingNote] = useState(null);

  useEffect(() => {
    const saved = loadState();
    setDone(saved.done);
    setNotes(saved.notes);
  }, []);

  useEffect(() => {
    saveState(done, notes);
  }, [done, notes]);

  const toggle = (key) => setDone((prev) => ({ ...prev, [key]: !prev[key] }));
  const totalProjects = phases.reduce((sum, phase) => sum + phase.projects.length, 0);
  const doneCount = Object.values(done).filter(Boolean).length;

  const resetAll = () => {
    if (!window.confirm("Reset all progress? This cannot be undone.")) {
      return;
    }

    setDone({});
    setNotes({});

    try {
      window.localStorage.removeItem(STORAGE_KEY);
    } catch {
      // Ignore storage failures so the UI still works.
    }
  };

  return (
    <div
      style={{
        fontFamily: "'IBM Plex Mono', 'SF Mono', 'Fira Code', monospace",
        background: "#0a0a0a",
        color: "#e4e4e7",
        minHeight: "100vh",
        padding: "32px 16px",
        boxSizing: "border-box",
      }}
    >
      <div style={{ maxWidth: 720, margin: "0 auto" }}>
        <div style={{ marginBottom: 40 }}>
          <div
            style={{
              fontSize: 11,
              letterSpacing: 4,
              color: "#71717a",
              textTransform: "uppercase",
              marginBottom: 8,
            }}
          >
            One LED · One Board · Complete Curriculum
          </div>
          <h1
            style={{
              fontSize: 28,
              fontWeight: 700,
              margin: 0,
              lineHeight: 1.2,
              color: "#fafafa",
            }}
          >
            STM32 Learning Roadmap
          </h1>
          <div
            style={{
              marginTop: 16,
              height: 6,
              background: "#27272a",
              borderRadius: 3,
              overflow: "hidden",
            }}
          >
            <div
              style={{
                height: "100%",
                width: `${(doneCount / totalProjects) * 100}%`,
                background:
                  "linear-gradient(90deg, #22c55e, #3b82f6, #a855f7, #f59e0b, #ef4444)",
                borderRadius: 3,
                transition: "width 0.4s ease",
              }}
            />
          </div>
          <div
            style={{
              display: "flex",
              justifyContent: "space-between",
              alignItems: "center",
              marginTop: 6,
            }}
          >
            <div style={{ fontSize: 12, color: "#71717a" }}>
              {doneCount} / {totalProjects} completed
            </div>
            <button
              onClick={resetAll}
              style={{
                fontSize: 11,
                color: "#52525b",
                background: "none",
                border: "none",
                cursor: "pointer",
                fontFamily: "inherit",
                textDecoration: "underline",
              }}
            >
              reset
            </button>
          </div>
        </div>

        {phases.map((phase) => {
          const isOpen = openPhase === phase.id;
          const phaseDone = phase.projects.filter((project) => done[project.name]).length;
          const phaseComplete = phaseDone === phase.projects.length;

          return (
            <div key={phase.id} style={{ marginBottom: 12 }}>
              <button
                onClick={() => setOpenPhase(isOpen ? null : phase.id)}
                style={{
                  width: "100%",
                  background: isOpen ? "#18181b" : "transparent",
                  border: `1px solid ${isOpen ? `${phase.color}66` : "#27272a"}`,
                  borderRadius: 8,
                  padding: "16px 20px",
                  cursor: "pointer",
                  display: "flex",
                  alignItems: "center",
                  gap: 16,
                  color: "#e4e4e7",
                  fontFamily: "inherit",
                  textAlign: "left",
                  transition: "all 0.2s ease",
                }}
              >
                <div
                  style={{
                    width: 40,
                    height: 40,
                    borderRadius: "50%",
                    border: `2px solid ${phase.color}`,
                    background: phaseComplete ? phase.color : "transparent",
                    display: "flex",
                    alignItems: "center",
                    justifyContent: "center",
                    fontSize: phaseComplete ? 18 : 16,
                    fontWeight: 700,
                    color: phaseComplete ? "#0a0a0a" : phase.color,
                    flexShrink: 0,
                    transition: "all 0.3s ease",
                  }}
                >
                  {phaseComplete ? "✓" : phase.id}
                </div>
                <div style={{ flex: 1 }}>
                  <div style={{ fontSize: 16, fontWeight: 600 }}>
                    {phase.title}: {phase.subtitle}
                  </div>
                  <div style={{ fontSize: 12, color: "#71717a", marginTop: 2 }}>
                    {phase.weeks} · {phaseDone}/{phase.projects.length} done
                  </div>
                </div>
                <div
                  style={{
                    fontSize: 18,
                    color: "#52525b",
                    transform: isOpen ? "rotate(180deg)" : "rotate(0deg)",
                    transition: "transform 0.2s ease",
                  }}
                >
                  ▾
                </div>
              </button>

              {isOpen && (
                <div
                  style={{
                    paddingLeft: 20,
                    borderLeft: `2px solid ${phase.color}33`,
                    marginLeft: 36,
                    marginTop: 8,
                  }}
                >
                  {phase.projects.map((project) => {
                    const isDone = done[project.name];
                    const isEditing = editingNote === project.name;
                    const note = notes[project.name] || "";

                    return (
                      <div
                        key={project.name}
                        style={{
                          padding: "14px 16px",
                          marginBottom: 6,
                          background: isDone ? `${phase.color}0d` : "#111113",
                          borderRadius: 6,
                          border: `1px solid ${isDone ? `${phase.color}44` : "#1e1e22"}`,
                          transition: "all 0.2s ease",
                        }}
                      >
                        <div style={{ display: "flex", alignItems: "flex-start", gap: 12 }}>
                          <button
                            onClick={() => toggle(project.name)}
                            style={{
                              width: 20,
                              height: 20,
                              borderRadius: 4,
                              flexShrink: 0,
                              marginTop: 1,
                              border: `2px solid ${isDone ? phase.color : "#3f3f46"}`,
                              background: isDone ? phase.color : "transparent",
                              cursor: "pointer",
                              display: "flex",
                              alignItems: "center",
                              justifyContent: "center",
                              color: "#0a0a0a",
                              fontSize: 12,
                              fontWeight: 700,
                              transition: "all 0.15s ease",
                            }}
                          >
                            {isDone ? "✓" : ""}
                          </button>
                          <div style={{ flex: 1 }}>
                            <div
                              style={{
                                fontSize: 14,
                                fontWeight: 600,
                                color: isDone ? phase.color : "#e4e4e7",
                                textDecoration: isDone ? "line-through" : "none",
                                opacity: isDone ? 0.7 : 1,
                              }}
                            >
                              {project.name}
                            </div>
                            <div
                              style={{
                                fontSize: 12,
                                color: "#a1a1aa",
                                marginTop: 6,
                                lineHeight: 1.6,
                              }}
                            >
                              {project.desc}
                            </div>
                            <div
                              style={{
                                fontSize: 11,
                                color: phase.color,
                                marginTop: 8,
                                padding: "4px 8px",
                                background: `${phase.color}11`,
                                borderRadius: 4,
                                display: "inline-block",
                              }}
                            >
                              * {project.key}
                            </div>
                            <div style={{ marginTop: 10 }}>
                              {isEditing ? (
                                <div>
                                  <textarea
                                    autoFocus
                                    value={note}
                                    onChange={(event) =>
                                      setNotes((prev) => ({
                                        ...prev,
                                        [project.name]: event.target.value,
                                      }))
                                    }
                                    placeholder="Your notes..."
                                    style={{
                                      width: "100%",
                                      minHeight: 60,
                                      background: "#0a0a0a",
                                      border: "1px solid #3f3f46",
                                      borderRadius: 4,
                                      color: "#e4e4e7",
                                      fontFamily: "inherit",
                                      fontSize: 12,
                                      padding: 8,
                                      resize: "vertical",
                                      boxSizing: "border-box",
                                    }}
                                  />
                                  <button
                                    onClick={() => setEditingNote(null)}
                                    style={{
                                      fontSize: 11,
                                      color: phase.color,
                                      background: "none",
                                      border: "none",
                                      cursor: "pointer",
                                      fontFamily: "inherit",
                                      marginTop: 4,
                                    }}
                                  >
                                    done
                                  </button>
                                </div>
                              ) : (
                                <button
                                  onClick={() => setEditingNote(project.name)}
                                  style={{
                                    fontSize: 11,
                                    color: note ? "#a1a1aa" : "#3f3f46",
                                    background: "none",
                                    border: "none",
                                    cursor: "pointer",
                                    fontFamily: "inherit",
                                    padding: 0,
                                    textAlign: "left",
                                    whiteSpace: "pre-wrap",
                                    lineHeight: 1.5,
                                  }}
                                >
                                  {note || "+ add notes"}
                                </button>
                              )}
                            </div>
                          </div>
                        </div>
                      </div>
                    );
                  })}
                </div>
              )}
            </div>
          );
        })}

        <div
          style={{
            textAlign: "center",
            fontSize: 11,
            color: "#3f3f46",
            marginTop: 40,
            paddingBottom: 20,
          }}
        >
          One LED is all you need.
        </div>
      </div>
    </div>
  );
}
