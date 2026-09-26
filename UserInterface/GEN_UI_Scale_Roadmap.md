# GEN UI XML+CSS — Roadmap Opción A (UI Scale / Zoom)

Plan de implementación por pasos **comprobables**.  
Validación principal: ejemplo **`Examples/Graphics/UI_System`** (build → ejecutar → capturas).

Contrato dual: **layouts sin stylesheet** (`UI_Options`) no cambian. Solo el camino XML+CSS / layouts con scale activo.

---

## Principio de diseño

| Concepto | Significado |
|----------|-------------|
| **Design px** | Coordenadas en las que se escribe XML/CSS (canvas lógico, p.ej. 1440×900) |
| **Framebuffer / screen px** | Píxeles reales de la ventana / superficie |
| **UIScale** | `scale = screen / design` (o zoom manual). Layout + hit-test en design; paint/input convierten |

**Regla de oro de validación:** si se cambia `scale` y el layout se ve/proporciona igual (más grande/pequeño) y los clics siguen acertando, el paso es correcto.

---

## Cómo validar cada fase en UI_System

1. Compilar `ui_system` (Windows intel64 Debug, mismo flujo habitual).
2. Ejecutar y navegar: Resumen → CPU → Memoria → Disco (charts / cards).
3. Capturar **siempre el mismo viewport** (p.ej. sección CPU con card + chart).
4. Guardar capturas en algo tipo:  
   `Examples/Graphics/UI_System/captures/uiscale/P0_baseline.png`, `P1_scale_1_5.png`, …
5. Comparar visualmente (y, si hace falta, hit-test manual: botones sidebar + cards).

---

## Fase 0 — Baseline (sin código de scale)

**Objetivo:** congelar “cómo se ve hoy” antes de tocar el motor.

| # | Paso | Comprobable |
|---|------|-------------|
| 0.1 | Documentar resolución de ventana / canvas actuales de UI_System | Nota en `captures/uiscale/README.md`: ancho×alto al arrancar |
| 0.2 | Captura baseline: Resumen, CPU, Memoria, Disco | 4 PNG con nombres `P0_*` |
| 0.3 | Lista de controles a hit-testear (sidebar + 2 botones card) | Checklist manual PASS/FAIL |
| 0.4 | Ejecutar `UnitTests_UserInterface` | 276 tests PASS (regresión lógica CSS) |

**Criterio de salida:** baseline + tests verdes. No se escribe aún UIScale.

---

## Fase 1 — API y contrato (design size + scale = 1)

**Objetivo:** existir el concepto sin cambiar el aspecto (scale 1.0 = idéntico al baseline).

| # | Paso | Comprobable |
|---|------|-------------|
| 1.1 | Añadir en `UI_MANAGER` o `UI_LAYOUT`: `designWidth`, `designHeight`, `uiScale` (default 1.0) | Unit test: getters/setters round-trip |
| 1.2 | Opt-in: solo layouts con stylesheet **o** flag explícito usan design canvas | `UI_Options` sin stylesheet: geometría igual (captura o smoke) |
| 1.3 | Documentar en comentario SCOPE / guía corta: “autores trabajan en design px” | Texto en header + este roadmap |
| 1.4 | UI_System con `scale=1.0` y design = tamaño actual del dashboard | Capturas `P1_scale1_*` **pixel-idénticas** (o indistinguibles) a `P0_*` |
| 1.5 | Unit tests nuevos: `UnitTests_UserInterface_UIScale.cpp` (defaults, clamp scale>0) | Google Test PASS |

**Estado:** API en `UI_LAYOUT` (`SetDesignSize` / `GetUIScale` / `IsUIScaleActive`). Opt-in = stylesheet o `SetUIScaleEnabled(true)`. CreateLayouts siembra design desde pantalla al cargar CSS. UI_System fija 1440×900 + scale 1.0.

**Criterio de salida:** API viva, UI_System se ve igual que P0, Options intacto.

---

## Fase 2 — Transformación de coordenadas (input + hit-test)

**Objetivo:** el puntero en screen px se convierte a design px antes del hit-test.

| # | Paso | Comprobable |
|---|------|-------------|
| 2.1 | Helper: `ScreenToDesign(x,y)` / `DesignToScreen(x,y)` | Unit tests con scale 1, 1.5, 2 |
| 2.2 | Cablear conversión en el camino de input (antes de `IsWithin` / pick) | Con scale forzado a 1.5 en prueba controlada: clic mapeado acierta AABB design |
| 2.3 | Layout/hit AABB siguen en design px (sin multiplicar geometría XML) | Unit test: AABB de elemento no cambia al cambiar solo scale |
| 2.4 | UI_System: scale 1.0 otra vez | Capturas `P2_scale1_*`; hit-test checklist PASS |

**Estado:** `UI_LAYOUT::ScreenToDesign` / `DesignToScreen`. `UI_MANAGER::MapScreenToDesign` en CURSOR_MOVE, SelectScrollBar y motion. AABB sin tocar. scale=1.0 = identidad visual.

**Criterio de salida:** hit-test correcto con scale ≠ 1 en prueba controlada; scale 1 sin regresión visual.

---

## Fase 3 — Transformación de paint (draw scale)

**Objetivo:** al pintar, el canvas lógico se escala al framebuffer (y opcional letterbox).

| # | Paso | Comprobable |
|---|------|-------------|
| 3.1 | Aplicar scale en el camino de composición SkinCanvas / blit final | Con scale 1.5: UI más grande, proporciones iguales |
| 3.2 | Política letterbox/pillarbox si aspect ratio screen ≠ design | Bordes negros/vacío simétricos documentados; captura `P3_letterbox` |
| 3.3 | Dirty-rect / restore-on-hide: invalidar en coords consistentes | UI_System: show/hide chart histórico CPU sin “agujeros” |
| 3.4 | Capturas UI_System: `scale=1.0`, `1.25`, `1.5` misma sección CPU | Tres PNG; layout reconocible; sin clipping absurdo de cards |
| 3.5 | Hit-test + paint juntos | Clic centro de botón a scale 1.5 activa la acción correcta |

**Estado:** design offscreen + `UIScale_Present` (Scale+blit+letterbox). Offsets en `ScreenToDesign`. Env `GEN_UI_SCALE` en UI_System. scale=1.0 = path identidad.

**Estado:** design offscreen + `UIScale_Present` (letterbox/crop). `ScreenToDesign` incluye offsets. Env `GEN_UI_SCALE` en UI_System.

**Criterio de salida:** zoom visual usable en UI_System; input alineado con lo dibujado.

---

## Fase 4 — Zoom de producto (runtime)

**Objetivo:** el usuario/app puede cambiar zoom (accesibilidad / preferencia).

| # | Paso | Comprobable |
|---|------|-------------|
| 4.1 | API runtime: `SetUIScale(s)` / `GetUIScale()` + reclamo de redraw | Llamada en caliente cambia tamaño sin reiniciar layout XML |
| 4.2 | Atajo o menú demo en UI_System (p.ej. `+` / `-` o teclas) | Capturas antes/después del mismo frame |
| 4.3 | Persistencia opcional (ini) | Reinicio de app restaura scale |
| 4.4 | Límites: `minScale` / `maxScale` (p.ej. 0.75…2.0) | Unit test clamp; UI no “desaparece” |

**Estado:** `Layouts_SetUIScale` + clamp `[0.75..2.0]`. Demo UI_System: numpad `+`/`−` y tecla `-` (paso 0.25). Persistencia `[general] uiscale` en `ui_system.ini` (env `GEN_UI_SCALE` sigue sobreescribiendo al arrancar).

**Criterio de salida:** zoom interactivo demostrable en UI_System con capturas.

---

## Fase 5 — Autoscale por ventana / DPI

**Objetivo:** al abrir en otra resolución, scale se calcula solo.

| # | Paso | Comprobable |
|---|------|-------------|
| 5.1 | `scale = min(sw/dw, sh/dh)` (fit) o `min`/`max` según política | Unit test con dw×dh fijos y pantallas ficticias |
| 5.2 | Recalcular al resize de ventana | Captura ventana pequeña vs grande; UI legible |
| 5.3 | Android (si aplica en el ciclo): mismo design; fit = GLES letterbox (UIScale 1.0) | Checklist `PP_android_notes.md` + captura dispositivo/emulador |
| 5.4 | UI_Options sigue sin scale automático | Smoke Options: sin stylesheet = comportamiento previo |

**Estado:** `ComputeFitUIScale` + `Layouts_ApplyFitUIScale`. UI_System: `uiscale_autofit` (ini, default sí); `CHANGESIZE` reaplica fit. Zoom `+/-` desactiva autofit. `UpdateSize` redimensiona canvas live. UI_Options sin cambios (XML-only = legacy).

**5.3 Android (Track P ✅):** design canvas fijo (p.ej. 1440×900); el fit a la superficie nativa lo hace el **letterbox GLES** (`GRPANDROIDBlitGLES` + `MapWindowToCanvas`). `UIScale` permanece en **1.0** (Present identidad) para no doble-escalar. `OnConfigurationChanged` (GLES) conserva el design size y emite `CHANGESIZE` sin `UpdateSize(native)`. Ver Track P en GEN_UI_CSS_Lite_Roadmap.md.

**Criterio de salida:** multi-resolución sin retocar `dashboard.xml`.

---

## Fase 6 — Min hit-target (complemento táctil)

**Objetivo:** iconos pequeños se pulsan bien sin redibujar más grandes.

| # | Paso | Comprobable |
|---|------|-------------|
| 6.1 | Política `minHitSize` en design px (p.ej. 44×44) expandiendo solo AABB de hit | Unit test: paint box 24×24, hit ≥ 44×44 |
| 6.2 | No cambia geometría de paint | Captura visual idéntica; solo mejora el clic |
| 6.3 | UI_System: scale bajo + iconos sidebar | Hit-test PASS en iconos estrechos |

**Estado:** `UI_LAYOUT::Get/SetMinHitSize` (default 44; 0 desactiva). `UI_BOUNDARYLINE::ExpandCenteredToMin`. `PreSelectElement` expande copia para `IsWithin` solo si `IsUIScaleActive`; paint AABB y clipping de scroll usan la caja original. UI_System fija `SetMinHitSize(44)` en dashboard. Layouts XML-only (UI_Options) no expanden.

**Criterio de salida:** táctil usable sin rediseñar assets.

---

## Fase 7 — Densidad de assets (nitidez)

**Objetivo:** no se vea borroso en HiDPI.

| # | Paso | Comprobable |
|---|------|-------------|
| 7.1 | Preferir SVG/vector donde ya exista | Cards/iconos nítidos a scale 1.5–2 |
| 7.2 | Bitmaps: raster al scale actual o @2x si hay pipeline | Captura zoom 2× sin pixelado extremo en SVG |
| 7.3 | StatisticsChart: rebuild/raster consciente del scale | Chart CPU legible a scale 1.5 |

**Estado:** overlay post-Present seguro (sin mutar BoundaryLine): SVG icons y StatisticsChart se rasterizan a `size×GetUIScale()` y se blitean sobre el live tras el Present. Design paint + hit-test intactos.

**Criterio de salida:** capturas HiDPI/zoom aceptables en UI_System.

---

## Fase 8 — Unidades vivas (puente hacia moldeabilidad)

**Objetivo:** aprovechar `UI_LENGTH` (rem/vw/vh) **después** de tener coordenadas lógicas.

| # | Paso | Comprobable |
|---|------|-------------|
| 8.1 | Cablear rem/vw/vh en builders vivos (`GetLayoutElement_Base` / RunLayout) | Unit tests de resolución con `UI_LENGTH_CONTEXT` ✅ |
| 8.2 | Demo puntual en `dashboard.css` (p.ej. un padding en `rem`) | Captura `P8_rem_sidebar.png` ✅ |
| 8.3 | Regresión suite CSS | `UnitTests_UserInterface` PASS (298) ✅ |

**Criterio de salida:** CSS más expresivo sin romper scale.

---

## Orden de merge / no hacer

| Hacer | No hacer aún |
|-------|----------------|
| Scale + input + paint juntos por fase | `@media` / breakpoints (Opción B) antes de A |
| Validar siempre UI_System con capturas | Romper layouts sin stylesheet |
| Unit tests en cada fase de API/math | Optimizar SVG chart (Phase 3 antigua) como prioridad |
| Mantener scale=1 como “modo compatible” | Mezclar letterbox + min-hit + rem en un solo PR enorme |

---

## Checklist rápida por PR sugerido

1. **PR-A1:** Fase 1 (API scale=1) + tests + capturas P1=P0  
2. **PR-A2:** Fase 2+3 (input+paint) + capturas scale 1.25/1.5 + hit-test  
3. **PR-A3:** Fase 4 (zoom runtime demo en UI_System)  
4. **PR-A4:** Fase 5 (autoscale resize/DPI)  
5. **PR-A5:** Fase 6+7 (min-hit + nitidez)  
6. **PR-A6:** Fase 8 (unidades vivas)  

Cada PR: build UI_System + capturas en `captures/uiscale/` + `UnitTests_UserInterface` verde.

---

## Definición de “terminado” Opción A

- Autores escriben UI_System (y futuros dashboards) en **design px** fijos.  
- En Windows/Linux/Android el UI **escala** sin reescribir XML.  
- Zoom manual funciona y el hit-test no se desvía.  
- Legacy XML-only intacto.  
- Hay **batería de capturas** reproducible y unit tests de la math de scale.

---

## Continuación: CSS Lite (post Opción A)

Backlog de autoría / layout / responsive / plataforma:  
**GEN_UI_CSS_Lite_Roadmap.md** (Tracks D, L, B, P, Q + puerta de producto).

