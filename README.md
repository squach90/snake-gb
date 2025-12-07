# 🐍 Snake

A classic Snake game developed in C with [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) for the Game Boy.

![Gameplay](./README-Assets/gameplay.gif)

## 📸 Screenshots

<div align="center">
  <img src="./README-Assets/mainmenu.png" alt="Main Menu" width="400"/>
  <img src="./README-Assets/gameplay.gif" alt="Gameplay" width="400"/>
  <img src="./README-Assets/gameover.png" alt="Game Over" width="400"/>
</div>

## 🎮 Features

- **Classic gameplay**: Control the snake and eat fruits to grow
- **Score system**: Earn 1 point per fruit collected
- **Collision detection**: Against borders and snake body
- **Sound effects**: Audio feedback when collecting fruits
- **User interface**: Real-time score display
- **Game Over screen**: Shows your final score

## 🕹️ Controls

| Button | Action |
|--------|--------|
| **D-Pad** | Move the snake (Up/Down/Left/Right) |
| **START** | Start game / Restart after Game Over |

## 🛠️ Building

### Prerequisites

- [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) installed
- Make (optional)

### Instructions

```bash
git clone https://github.com/squach90/snake-gb.git
make
```

## 🎯 How to Play

1. **Start**: Press START on the title screen
2. **Objective**: Eat fruits (🍎) to grow your snake
3. **Avoid**:
   - The field borders
   - Your own body
4. **Score**: Each fruit gives 1 point
5. **Game Over**: Press START to restart

## 📊 Technical Details

- **Language**: C
- **SDK**: GBDK
- **Field size**: 20x17 tiles
- **Speed**: 7 tiles per second
- **Maximum length**: 200 segments
- **RNG**: Custom pseudo-random number generator

## 🎨 Assets

The game uses custom tiles for:

- Snake (head and body)
- Fruits
- Borders
- Digits (0-9)
- Letters (A-Z)

## 🔧 Code Features

- **Collision management**: Precise detection with body and borders
- **Direction system**: Prevents impossible 180° turns
- **Random generation**: Fruit placement without collision with snake
- **Audio**: Uses Game Boy sound registers (NR10-NR14)
- **Double buffer**: Window Layer usage for score display

## 🐛 Known Limitations

- Maximum score is 99 (2-digit display)
- Maximum snake length is 200 segments

## 🚀 Future Improvements

- [ ] Saved high score
- [ ] Additional Sprite for Fruit
- [ ] Background music

## 👀 🫵 ⏭️ ⏰ ヾ(＾ ∇ ＾).

### Boring stuff

This project is released under the [Unlicense](./UNLICENSE).

Read more on [my blog](https://squach90.github.io)

**Developed with ❤️ by [@squach90](https://github.com/squach90)**

---

> **Note**: This game is compatible with Game Boy emulators (BGB, VBA, etc.) and flashcart cartridges for playing on original hardware.
