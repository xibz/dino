#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <stdio.h>
#include <string>
#include "emu.h"

void renderWindow(void *);

void renderWindow(void *w)
{
  sf::RenderWindow *window = (sf::RenderWindow *)w;
  while(window->isOpen())
  {
    sf::Event event;
    while(window->pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
        window->close();
    }
    window->clear();
    window->display();
  }
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Error: usage ./dino <rom name>\n");
    return 0;
  }
  sf::RenderWindow window(sf::VideoMode(800,600), argv[1]);
  sf::Thread graphics(&renderWindow, &window);
  window.setActive(false);
  graphics.launch();
  std::string romName = argv[1]; 
  Emu emu(romName);
  graphics.wait();
  return 0;
}
