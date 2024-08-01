class ProcessLanguages:
    languages: set = set()

    @staticmethod
    def add_language(language: str) -> None:
        ProcessLanguages.languages.add(language)
