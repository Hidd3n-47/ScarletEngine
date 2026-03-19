from dataclasses import dataclass, field
from Component import Component

@dataclass
class Entity:
    mutable: bool = False
    components: list[Component] = field(default_factory=list)